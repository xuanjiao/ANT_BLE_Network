/*
This software is subject to the license described in the License.txt file
included with this software distribution. You may not use this file except
in compliance with this license.

Copyright (c) Dynastream Innovations Inc. 2016
All rights reserved.
*/
#include "types.h"
#if defined(DSI_TYPES_MACINTOSH)

#include "iokit_device_handle.hpp"

#include "dsi_convert.h"
#include "dsi_debug.hpp"
#include "macros.h"
#include "dsi_thread.h"

#include "iokit_device_list.hpp"

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <mach/clock.h>
#include <mach/clock_types.h>
#include <mach/mach_host.h>

#include <mach/mach_port.h>
#include <IOKit/IOCFBundle.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/IOCFPlugIn.h>

#include <memory>
using namespace std;


//////////////////////////////////////////////////////////////////////////////////
// Public Methods
//////////////////////////////////////////////////////////////////////////////////

//!!Need to handle hot removal (cancel transfers, exit transfers, close)

///////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////
IOKitDeviceHandle::IOKitDeviceHandle(const IOKitDevice& dev) throw (IOKitError::Enum)  //should throw a reference to a sibling of exception class
: device(dev)
{
   thread_started = FALSE;
   thread = NULL;
   usb_device = NULL;
   run_loop = NULL;
   event_source = NULL;
   mach_port =  0;

   int r;
   r = pthread_mutex_init(&lock, NULL);
   if(r != 0)
      throw IOKitError::OTHER;

   if(pthread_mutex_init(&lock_runloop, NULL) != 0)
   {
      pthread_mutex_destroy(&lock);
      throw IOKitError::OTHER;
   }

   if(pthread_cond_init(&cond_runloop, NULL) != 0)
   {
      pthread_mutex_destroy(&lock_runloop);
      pthread_mutex_destroy(&lock);
      throw IOKitError::OTHER;
   }

   if(pthread_mutex_init(&lock_transfer_list, NULL) != 0)
   {
      pthread_cond_destroy(&cond_runloop);
      pthread_mutex_destroy(&lock_runloop);
      pthread_mutex_destroy(&lock);
      throw IOKitError::OTHER;
   }

   if(pthread_mutex_init(&lock_ep_in_lock, NULL) != 0)
   {
      pthread_mutex_destroy(&lock_transfer_list);
      pthread_cond_destroy(&cond_runloop);
      pthread_mutex_destroy(&lock_runloop);
      pthread_mutex_destroy(&lock);
      throw IOKitError::OTHER;
   }

   if(pthread_mutex_init(&lock_ep_other_lock, NULL) != 0)
   {
      pthread_mutex_destroy(&lock_ep_in_lock);
      pthread_mutex_destroy(&lock_transfer_list);
      pthread_cond_destroy(&cond_runloop);
      pthread_mutex_destroy(&lock_runloop);
      pthread_mutex_destroy(&lock);
      throw IOKitError::OTHER;
   }

   transfer_list.clear();
   areTransfersClosed = FALSE;



   //////////////////////////////////////////////////


   //Create master port for talking to IOKit
   IOReturn kresult = IOMasterPort(MACH_PORT_NULL, &mach_port);
   if(kresult != kIOReturnSuccess || mach_port == 0)
   {
      PClose();
      throw darwin_to_libusb(kresult);
   }

   IOKitError::Enum result;
   result = TryOpen();
   if(result < 0)
   {
      //!!Check Close() for how to handle these.
      PClose();
      throw result;
   }

   pthread_mutex_lock(&lock_runloop);
   if(pthread_create(&thread, NULL, &IOKitDeviceHandle::EventThreadStart, this) != 0)
   {
      pthread_mutex_unlock(&lock_runloop);
      PClose();
      throw IOKitError::OTHER;
   }

   //wait for thread to signal back
   pthread_cond_wait(&cond_runloop, &lock_runloop);  //!!have a timeout


   #if defined(DEBUG_FILE)
   if(!thread_started)
      DSIDebug::ThreadWrite("IOKitDeviceHandle::nIOKitDeviceHandle: thread_started NULL!");
   if(!event_source)
      DSIDebug::ThreadWrite("IOKitDeviceHandle::nIOKitDeviceHandle: event_source NULL!");
   #endif

   if(!thread_started || !event_source) // If the run loop is not setup, we are broken
   {
      PClose();
      throw IOKitError::OTHER;
   }

   CFRunLoopAddSource(run_loop, event_source, kCFRunLoopCommonModes);

   pthread_mutex_unlock(&lock_runloop);

   return;
}

///////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////
IOKitDeviceHandle::~IOKitDeviceHandle()
{
   PClose();
}

void IOKitDeviceHandle::PClose()
{
   if(areTransfersClosed != TRUE)
   {
      //First ensure all transfers are cancelled before we are safe to destroy resources
       pthread_mutex_lock(&lock_transfer_list); //Need list lock to ensure new transfers see the closed flag and so we can iterate through existing transfers

      areTransfersClosed = TRUE;   //No new transfers will be allowed on this device, device is closed


      for (std::list<IOKitTransfer*>::iterator i = transfer_list.begin(); i != transfer_list.end(); i++)
         (*i)->Cancel();

      pthread_mutex_unlock(&lock_transfer_list);

      //Try and wait for all the transfers to finish
      for(int i = 0; i < 15; ++i)
      {
         pthread_mutex_lock(&lock_transfer_list);
         if(transfer_list.size() <= 0)
         {
            break;
         }
         else
         {
            if(i == 14) //If for some reason a transfer is still stalled we have to force kill it to clean up resources, this may cause a crash but there is not much else we can do...
            {
               for (std::list<IOKitTransfer*>::iterator i = transfer_list.begin(); i != transfer_list.end(); i++)
                  delete *i;
               transfer_list.clear();
            }
            else
            {
               DSIThread_Sleep(100);
            }
         }
         pthread_mutex_unlock(&lock_transfer_list);
      }
   }

    //Now we can safely close resources

    //stop the async runloop
    if(run_loop != NULL)
    {
        if(event_source != NULL)
        {
            if(CFRunLoopContainsSource(run_loop, event_source, kCFRunLoopDefaultMode))
                CFRunLoopRemoveSource(run_loop, event_source, kCFRunLoopDefaultMode);  //!!Should each device be on their own mode?
        }
        CFRunLoopStop(run_loop);

        pthread_join(thread, nullptr); //wait for runloop to finish
        run_loop = NULL;
    }

    //delete the device's async event source
    if(event_source != NULL)
    {
        CFRelease(event_source);
        event_source = NULL;
    }

   //make sure all interfaces are released
   for(int i = 0; i < IOKitDeviceHandle::MAX_INTERFACES; i++)
      ReleaseInterface(i);

   if(usb_device != NULL)
   {
      (*(usb_device))->USBDeviceClose(usb_device);
      (*(usb_device))->Release(usb_device);
      usb_device = NULL;
   }

   if(mach_port != 0)
   {
      mach_port_deallocate(mach_task_self(), mach_port);
      mach_port = 0;
   }

   if(!areTransfersClosed)
   {
      pthread_mutex_destroy(&lock_transfer_list);
      pthread_mutex_destroy(&lock_ep_in_lock);
      pthread_mutex_destroy(&lock_ep_other_lock);
      pthread_cond_destroy(&cond_runloop);
      pthread_mutex_destroy(&lock_runloop);
      pthread_mutex_destroy(&lock);
   }
}


IOKitError::Enum IOKitDeviceHandle::Open(const IOKitDevice& dev, IOKitDeviceHandle*& dev_handle)
{
   try
   {
      dev_handle = new IOKitDeviceHandle(dev);
   }
   catch(IOKitError::Enum err)
   {
      return err;
   }
   catch(bad_alloc err)
   {
      return IOKitError::NO_MEM;
   }
   catch(...)
   {
      return IOKitError::OTHER;
   }

   if(dev_handle == NULL)  //!! don't need this
      return IOKitError::NO_MEM;

   return IOKitError::SUCCESS;
}

//this needs to be run when a receive thread is pending on a message to its pipe from the IOKitTransfer object
//IOKitDeviceHandle::Close should not be cancelling transfers as it is run AFTER the receive thread has been destroyed
void IOKitDeviceHandle::CancelQueuedTransfers(void)
{
   std::list<IOKitTransfer*>::iterator cancel_iterator;

   pthread_mutex_lock(&lock_transfer_list);
   for (cancel_iterator = transfer_list.begin(); cancel_iterator != transfer_list.end(); cancel_iterator++)
      (*cancel_iterator)->Cancel();
   pthread_mutex_unlock(&lock_transfer_list);
}

//!!Should we return an error code?
void IOKitDeviceHandle::Close(IOKitDeviceHandle*& dev_handle)
{
   if(dev_handle == NULL)
      return;

   dev_handle->PClose();

  delete dev_handle;
  dev_handle = NULL;

  return;
}


//Hard reset
//We have to do full close because transfers must be cancelled or they will blow up
IOKitError::Enum IOKitDeviceHandle::Reset(IOKitDeviceHandle*& dev_handle)
{
   if(dev_handle == NULL)
      return IOKitError::INVALID_PARAM;

   dev_handle->PClose();

   delete dev_handle;
   dev_handle = NULL;

   return IOKitError::SUCCESS;
}


IOKitError::Enum IOKitDeviceHandle::ClaimInterface(int interface_number)
{

   IOKitError::Enum ret;

   //make sure the interface is within the claimed interface number
   if(interface_number >= IOKitDeviceHandle::MAX_INTERFACES)
      return IOKitError::INVALID_PARAM;

   pthread_mutex_lock(&lock);
   ret = interfaces[interface_number].Claim(*this, interface_number);
   pthread_mutex_unlock(&lock);

   return ret;
}

IOKitError::Enum IOKitDeviceHandle::ReleaseInterface(int interface_number)
{
   IOKitError::Enum ret;

   //make sure the interface is within the claimed interface number
   if(interface_number >= IOKitDeviceHandle::MAX_INTERFACES)
      return IOKitError::INVALID_PARAM;

   pthread_mutex_lock(&lock);
   ret = interfaces[interface_number].Release();
   pthread_mutex_unlock(&lock);
   return ret;

}

IOKitError::Enum IOKitDeviceHandle::Write(unsigned char endpoint, unsigned char* data, int length, int& actual_length, unsigned int timeout)
{
   return BulkTransfer(endpoint, data, length, timeout, actual_length);
}

IOKitError::Enum IOKitDeviceHandle::Read(unsigned char endpoint, unsigned char* data, int length, int& actual_length, unsigned int timeout)
{
   return BulkTransfer(endpoint, data, length, timeout, actual_length);
}

IOKitError::Enum IOKitDeviceHandle::WriteControl(unsigned char requestType, unsigned char request, unsigned short value, unsigned short index, unsigned char* data, int length, int& actual_length, unsigned int timeout)
{
   return SendSyncControlTransfer(requestType, request, value, index, data, length, timeout, actual_length);
}

//Retrieve a string descriptor in C style ASCII.
//Wrapper around libusb_get_string_descriptor(). Uses the first language
//supported by the device.
IOKitError::Enum IOKitDeviceHandle::GetStringDescriptorAscii(uint8_t desc_index, unsigned char data[], int length, int& transferred)
{

   IOKitError::Enum r;


   int string_length;
   string_descriptor str_desc_zero;
   r = GetStringDescriptor(0, 0, str_desc_zero, string_length);
   if (r != IOKitError::SUCCESS)
      return r;

   //make sure at least one language is supported
   if(string_length < 4)
      return IOKitError::IO;

   //take first language
   USHORT langid = Convert_Bytes_To_USHORT(str_desc_zero.lang_id[0].second_byte, str_desc_zero.lang_id[0].first_byte);

   string_descriptor str_desc_one;
   r = GetStringDescriptor(desc_index, langid, str_desc_one, string_length);
   if(r != IOKitError::SUCCESS)
      return r;

   if(str_desc_one.descriptor_type != LIBUSB_DT_STRING)
      return IOKitError::IO;

   if(str_desc_one.length > string_length)
      return IOKitError::IO;

   if(str_desc_one.length > 128)
      str_desc_one.length = 128;

   //Convert from unicode to ascii
   int si, di;
   for(di = 0, si = 0; si < str_desc_one.length; si += 2)
   {
      if(di >= (length - 1))
         break;

      if(str_desc_one.string[si + 1]) //high byte
         data[di] = '?';
      else
         data[di] = str_desc_one.string[si];

      di++;
   }
   data[di] = '\0';


   transferred = str_desc_one.length;
   return IOKitError::SUCCESS;

}

IOKitError::Enum IOKitDeviceHandle::GetDescriptor(uint8_t desc_type, uint8_t desc_index, unsigned char data[], int length, int& transferred)
{
   return SendSyncControlTransfer(LIBUSB_ENDPOINT_IN,
      LIBUSB_REQUEST_GET_DESCRIPTOR, (desc_type << 8) | desc_index, 0, data,
      length, 1000, transferred);
}


IOKitError::Enum IOKitDeviceHandle::GetStringDescriptor(uint8_t desc_index, uint16_t langid, string_descriptor& str_desc, int& transferred)
{
   const int length = sizeof(str_desc);
   unsigned char* data = reinterpret_cast<unsigned char*>(&str_desc);
   return SendSyncControlTransfer(LIBUSB_ENDPOINT_IN,
      LIBUSB_REQUEST_GET_DESCRIPTOR, (LIBUSB_DT_STRING << 8) | desc_index,
      langid, data, length, 1000, transferred);
}



//////////////////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////////////////
#define ANT_USB_STICK_VID 0x0FCF
IOKitError::Enum IOKitDeviceHandle::TryOpen()
{

  IOReturn kresult;

  if(device.usVid != ANT_USB_STICK_VID)   // Check VID before allocating resources to connect to USB stick
     return IOKitError::OTHER;

  usb_device = device.CreateDeviceInterface(device.hSavedService);

  if (!usb_device)
     return IOKitError::NO_DEVICE;

  //try to open the device
  kresult = (*usb_device)->USBDeviceOpen(usb_device);
  if(kresult != kIOReturnSuccess)
  {
     IOReturn other_result;
      other_result = (*usb_device)->USBDeviceClose(usb_device);
      if(other_result) {} //error

      other_result = (*usb_device)->Release(usb_device);
      if(other_result) {} //error

      usb_device = NULL;
      return darwin_to_libusb (kresult);
  }
  else
  {
     //create async event source
     kresult = (*usb_device)->CreateDeviceAsyncEventSource(usb_device, &event_source);
     if(kresult != kIOReturnSuccess)
        return darwin_to_libusb(kresult);
  }

   return IOKitError::SUCCESS;
}


kern_return_t IOKitDeviceHandle::GetDevice(uint32_t dev_location, IOKitDevice& clDevice_)
{
  kern_return_t kresult;


  io_iterator_t deviceIterator;
  kresult = IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching(kIOUSBDeviceClassName), &deviceIterator);
  if (kresult)
    return kresult;

  //This port of libusb uses locations to keep track of devices.

  io_service_t hService;
  while ((hService = IOKitDeviceList::GetNextDevice(deviceIterator)) != 0)
  {
   IOKitDevice clDevice(hService);
    if (clDevice.ulLocation == dev_location)
   {
      clDevice_ = clDevice;
      IOObjectRelease(hService);
      break;
   }

   IOObjectRelease(hService);
  }

  IOObjectRelease(deviceIterator);

  if(!hService)
    return kIOReturnNoDevice;

  return kIOReturnSuccess;
}







IOKitError::Enum IOKitDeviceHandle::InterruptTransfer(unsigned char endpoint, unsigned char *data, int length, unsigned int timeout, int& transferred)
{
   return SendSyncInterruptTransfer(endpoint, data, length, timeout, transferred);
}


IOKitError::Enum IOKitDeviceHandle::BulkTransfer(unsigned char endpoint, unsigned char *data, int length, unsigned int timeout, int& transferred)
{
   return SendSyncBulkTransfer(endpoint, data, length, timeout, transferred);
}




IOKitError::Enum IOKitDeviceHandle::SendSyncControlTransfer(uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout, int& transferred)
{
   unsigned char* buffer = NULL;
   if (wLength)
   {
      buffer = new unsigned char[wLength];

      if(!buffer)
         return IOKitError::NO_MEM;
   }


   //fill control setup
   struct libusb_control_setup setup;
   setup.bmRequestType = bmRequestType;
   setup.bRequest = bRequest;
   setup.wValue = libusb_cpu_to_le16(wValue);
   setup.wIndex = libusb_cpu_to_le16(wIndex);
   setup.wLength = libusb_cpu_to_le16(wLength);

   //fill buffer if we are writing to the device
   BOOL isRead = TRUE;
   if((bmRequestType & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_OUT)
   {
      isRead = FALSE;
      if(wLength)
         memcpy(buffer, data, wLength);
   }

   //!!Could use factory and auto_ptr<>() if not in an STL container
   IOKitTransfer* transfer = IOKitTransfer::MakeControlTransfer(*this, setup, buffer, timeout);

   IOKitError::Enum ret = handleTransfer(transfer, isRead, transferred, timeout);

   delete[] buffer;

   return ret;
}

IOKitError::Enum IOKitDeviceHandle::SendSyncInterruptTransfer(unsigned char endpoint, unsigned char *buffer, int length, unsigned int timeout, int& transferred)
{
   //!!Could use factory and auto_ptr<>() if not in an STL container
   IOKitTransfer* transfer = IOKitTransfer::MakeInterruptTransfer(*this, endpoint, buffer, length, timeout);

   return handleTransfer(transfer, endpoint & LIBUSB_ENDPOINT_IN, transferred, timeout);
}


IOKitError::Enum IOKitDeviceHandle::SendSyncBulkTransfer(unsigned char endpoint, unsigned char *buffer, int length, unsigned int timeout, int& transferred)
{
   //!!Could use factory and auto_ptr<>() if not in an STL container
   IOKitTransfer* transfer = IOKitTransfer::MakeBulkTransfer(*this, endpoint, buffer, length, timeout);

   return handleTransfer(transfer, endpoint & LIBUSB_ENDPOINT_IN, transferred, timeout);
}

IOKitError::Enum IOKitDeviceHandle::handleTransfer(IOKitTransfer* transfer, BOOL isRead, int& transferred, unsigned int timeout)
{
   IOKitError::Enum ret = IOKitError::OTHER;
   TransferStatus::Enum status = TransferStatus::ERROR;

   //TODO subtract locking time from requested timeout?

   pthread_mutex_lock(&lock_transfer_list);

   //Make sure we don't queue any transfers after we are closed
   if (areTransfersClosed == TRUE)
   {
      delete transfer;
      return IOKitError::TIMEOUT; //There is no 'cancelled' return defined, when active transfers are canceled they return 'timeout' so we match that.
   }
   else
   {
      //Place onto transfer list so it will see any cancel requests
      transfer_list.push_back(transfer);
   }

   pthread_mutex_unlock(&lock_transfer_list);


   if(isRead)
      pthread_mutex_lock(&lock_ep_in_lock);
   else
      pthread_mutex_lock(&lock_ep_other_lock);


   //If the transfer is submitted successfully, wait for the result
   //Note: don't need to check cancel again because submit returns TIMEOUT if transfer was already cancelled
   if((ret = transfer->Submit()) == IOKitError::SUCCESS)
   {
      status = transfer->Receive();

      if(status == TransferStatus::INTERRUPTED)
         transfer->Cancel();

      transferred = transfer->GetDataLength();

      switch(status)
      {
         case TransferStatus::COMPLETED:
            ret = IOKitError::SUCCESS;
            break;
         case TransferStatus::CANCELLED:
         case TransferStatus::TIMED_OUT:
            ret = IOKitError::TIMEOUT;
            break;
         case TransferStatus::STALL:
            ret = IOKitError::PIPE;
            break;
         case TransferStatus::OVERFLOWED:
            ret = IOKitError::OVERFLOWED;
            break;
         case TransferStatus::NO_DEVICE:
            ret = IOKitError::NO_DEVICE;
            break;
         default:
            //unrecognised status code
            ret = IOKitError::OTHER;
            break;
      }
   }

   if(isRead)
      pthread_mutex_unlock(&lock_ep_in_lock);
   else
      pthread_mutex_unlock(&lock_ep_other_lock);

   pthread_mutex_lock(&lock_transfer_list);
   transfer_list.remove(transfer);  //delete transfer object from list now that it is done
   pthread_mutex_unlock(&lock_transfer_list);

   delete transfer;
   return ret;
}



































void IOKitDeviceHandle::DevicesDetached(void* param, io_iterator_t removed_devices)
{

  IOKitDeviceHandle* This = reinterpret_cast<IOKitDeviceHandle*>(param);

   //See if this device is in the list of removed devices
  io_service_t device;
  while ((device = IOIteratorNext(removed_devices)) != 0)
  {
    /* get the location from the i/o registry */
    CFTypeRef locationCF;
    locationCF = IORegistryEntryCreateCFProperty(device, CFSTR(kUSBDevicePropertyLocationID), kCFAllocatorDefault, 0);

    long location;
    CFNumberGetValue(reinterpret_cast<CFNumberRef>(locationCF), kCFNumberLongType, &location);
    CFRelease(locationCF);
    IOObjectRelease(device);

    if( static_cast<long>( This->device.ulLocation ) == location)
      break;
  }

  //!!Need to release iterator?

  //!!Right now we just let the return code from user calls tell the user about the detached device
  //We could also have them specify a function to call when a device is detached.
  /*
  if(device != 0) //we are the device that has disconnected
      IOKitDeviceHandle::Close(This);
   */

   return;
}

void* IOKitDeviceHandle::EventThreadStart(void* param_)
{
   IOKitDeviceHandle* This = reinterpret_cast<IOKitDeviceHandle*>(param_);
   This->EventThread();
   return NULL;
}

void IOKitDeviceHandle::EventThread()
{
   IOReturn kresult;
   io_iterator_t removed_devs_iterator;

   //hotplug (device removal) source
   CFRunLoopSourceRef notification_source;
   io_notification_port_t notification_port;

   run_loop = CFRunLoopGetCurrent();

   CFRetain(run_loop);

   //add the notification port to the run loop
   try
   {
      if (!mach_port)
      {
         #if defined(DEBUG_FILE)
            DSIDebug::ThreadWrite("IOKitDeviceHandle::EventThread: mach_port NULL!");
         #endif
         throw;
      }
      notification_port = IONotificationPortCreate(mach_port);
      if (!notification_port)
      {
         #if defined(DEBUG_FILE)
            DSIDebug::ThreadWrite("IOKitDeviceHandle::EventThread: notification_port NULL!");
         #endif
         throw;
      }
      notification_source = IONotificationPortGetRunLoopSource(notification_port);
      if (!notification_source)
      {
         #if defined(DEBUG_FILE)
            DSIDebug::ThreadWrite("IOKitDeviceHandle::EventThread: notification_source NULL!");
         #endif
         throw;
      }
      CFRunLoopAddSource(run_loop, notification_source, kCFRunLoopDefaultMode);

      //create notifications for removed devices
      kresult = IOServiceAddMatchingNotification(  notification_port,
                                                   kIOTerminatedNotification,
                                                   IOServiceMatching(kIOUSBDeviceClassName),
                                                   (IOServiceMatchingCallback)IOKitDeviceHandle::DevicesDetached,
                                                   this,
                                                   &removed_devs_iterator
                                                );
      if(kresult != kIOReturnSuccess)
      {
         #if defined(DEBUG_FILE)
            DSIDebug::ThreadWrite("IOKitDeviceHandle::EventThread: IOServiceAddMatchingNotification failed");
         #endif
         throw;
      }
   }
   catch(...)
   {
      #if defined(DEBUG_FILE)
         DSIDebug::ThreadWrite("IOKitDeviceHandle::EventThread: within catch( ... )");
      #endif

      CFRunLoopSourceInvalidate(notification_source);
      IONotificationPortDestroy(notification_port);
      //!!Need to release iterator?

      pthread_mutex_lock(&lock_runloop);

      CFRelease(run_loop);
      thread_started = FALSE;

      pthread_cond_signal(&cond_runloop);
      pthread_mutex_unlock(&lock_runloop);

      return;
   }

   //arm notifiers
   //clear iterator
   io_service_t localdevice;
   while ((localdevice = IOIteratorNext(removed_devs_iterator)) != 0)
     IOObjectRelease (localdevice);

   /* let the main thread know about the async runloop */
   pthread_mutex_lock(&lock_runloop);
   thread_started = TRUE;
   pthread_cond_signal(&cond_runloop);
   pthread_mutex_unlock(&lock_runloop);

   /* run the runloop */
   CFRunLoopRun();

   /* delete notification port */
   CFRunLoopSourceInvalidate(notification_source);
   IONotificationPortDestroy(notification_port);
   IOObjectRelease(removed_devs_iterator);

   pthread_mutex_lock(&lock_runloop);
   CFRelease(run_loop);
   thread_started = FALSE;
   pthread_cond_signal(&cond_runloop);
   pthread_mutex_unlock(&lock_runloop);

   pthread_exit(0);
}


#endif //defined(DSI_TYPES_MACINTOSH)

