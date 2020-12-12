
#include "device.h"

#include <QMainWindow>

static UCHAR channel_to_setID = 0;
static UCHAR channel_to_setRF = 0;
static UCHAR channel_to_open = 0;

static BOOL beacon_exist = FALSE;

Device::Device()
{
    ucChannelType = CHANNEL_TYPE_INVALID;
    pclSerialObject = (DSISerialGeneric*)NULL;
    pclMessageObject = (DSIFramerANT*)NULL;
    uiDSIThread = (DSI_THREAD_ID)NULL;
    bMyDone = FALSE;
    bDone = FALSE;
    bDisplay = TRUE;
    bBroadcasting = FALSE;

    memset(aucTransmitBuffer,0,ANT_STANDARD_DATA_PAYLOAD_SIZE);
}

Device::~Device(){
    if(pclMessageObject)
       delete pclMessageObject;

    if(pclSerialObject)
       delete pclSerialObject;
}


void Device::storeConfig(UCHAR ucChType, UCHAR ucUSBDeviceNum, UCHAR ucDeviceNum, UCHAR ucDeviceType, UCHAR ucTransType, UCHAR ucRF){

    this->ucChannelType = ucChType;
    this->ucUSBDeviceNum = ucUSBDeviceNum;
    this->ucDeviceNum =  ucDeviceNum;
    this->ucDeviceType = ucDeviceType;
    this->ucTransType = ucTransType;
    this->ucRF = ucRF;

    qDebug("config: channel type %x RF %d devNum %x devType %x tranType %x USB %d",
           this->ucChannelType,
           this->ucRF,
           this->ucDeviceNum,
           this->ucDeviceType,
           this->ucTransType,
           this->ucUSBDeviceNum);
}

BOOL Device::Init(UCHAR ucDeviceNumber_, UCHAR ucChannelType_)
{

   BOOL bStatus;

   // Initialize condition var and mutex
   UCHAR ucCondInit = DSIThread_CondInit(&condTestDone);


   UCHAR ucMutexInit = DSIThread_MutexInit(&mutexTestDone);

#if defined(DEBUG_FILE)
   // Enable logging
   DSIDebug::Init();
   DSIDebug::SetDebug(TRUE);
#endif

   // Create Serial object.
   pclSerialObject = new DSISerialGeneric();
   if(!pclSerialObject){
       qDebug()<<"create serial object failed";
       return FALSE;
   }
   ucChannelType = ucChannelType_;

   // Initialize Serial object.
   // The device number depends on how many USB sticks have been
   // plugged into the PC. The first USB stick plugged will be 0
   // the next 1 and so on.
   //
   // The Baud Rate depends on the ANT solution being used. AP1
   // is 50000, all others are 57600
   bStatus = pclSerialObject->Init(USER_BAUDRATE, ucDeviceNumber_);
   if(!bStatus){
       qDebug()<<"create pclSerialObject failed";
       return FALSE;
   }

   // Create Framer object.
   pclMessageObject = new DSIFramerANT(pclSerialObject);
   if(!pclMessageObject){
       qDebug()<<"create pclMessageObject failed";
       return FALSE;
   }

   // Initialize Framer object.
   bStatus = pclMessageObject->Init();
   if(!bStatus){
       qDebug()<<"pclMessageObject init failed";
       return FALSE;
   }

   // Let Serial know about Framer.
   pclSerialObject->SetCallback(pclMessageObject);

   // Open Serial.
   bStatus = pclSerialObject->Open();

   // If the Open function failed, most likely the device
   // we are trying to access does not exist, or it is connected
   // to another program
   if(!bStatus)
   {
      qDebug("Failed to connect to device at USB port %d\n", ucDeviceNumber_);
      return FALSE;
   }

   // Create message thread.
   uiDSIThread = DSIThread_CreateThread(&Device::RunMessageThread, this);
   if(!uiDSIThread){
       qDebug()<<"create uiDSIThread failed";
       return FALSE;
   }

  qDebug("Initialization was successful!\n");

   return TRUE;
}

void Device::Close()
{
   //Wait for test to be done
   DSIThread_MutexLock(&mutexTestDone);
   bDone = TRUE;
   BOOL bStatus;
   UCHAR ucWaitResult = DSIThread_CondTimedWait(&condTestDone, &mutexTestDone, DSI_THREAD_INFINITE);
   if(ucWaitResult != DSI_THREAD_ENONE){
       qDebug()<<"Fail: ucWaitResult ="<<ucWaitResult;
   }

   if(isMultiChannel){
       for(int i = 0; i < MAX_CHANNEL_NUM;i++){
            bStatus = pclMessageObject->UnAssignChannel(i,0);
            qDebug()<<"unsign channel "<<i<<" result "<<bStatus;
       }
   }else{
       bStatus = pclMessageObject->UnAssignChannel(USER_ANTCHANNEL,0);
       qDebug()<<"unsign channel "<<USER_ANTCHANNEL<<" result "<<bStatus;
   }


   DSIThread_MutexUnlock(&mutexTestDone);

   //Destroy mutex and condition var
   DSIThread_MutexDestroy(&mutexTestDone);
   DSIThread_CondDestroy(&condTestDone);

   //Close all stuff
   if(pclSerialObject)
      pclSerialObject->Close();

#if defined(DEBUG_FILE)
   DSIDebug::Close();
#endif

}

BOOL Device::InitResetANT(void)
{
   BOOL bStatus;

   // Reset system
   qDebug("Resetting module...\n");
   bStatus = pclMessageObject->ResetSystem();
   DSIThread_Sleep(1000);

   // Start the test by setting network key
   qDebug("Setting network key...\n");
   UCHAR ucNetKey[8] = USER_NETWORK_KEY;

   bStatus = pclMessageObject->SetNetworkKey(USER_NETWORK_NUM, ucNetKey, MESSAGE_TIMEOUT);

   return bStatus;
}


DSI_THREAD_RETURN Device::RunMessageThread(void *pvParameter_)
{
   ((Device*) pvParameter_)->MessageThread();
   return NULL;
}

void Device::MessageThread()
{
   ANT_MESSAGE stMessage;
   USHORT usSize;
   bDone = FALSE;

   while(!bDone)
   {
      if(pclMessageObject->WaitForMessage(1000))
      {
         usSize = pclMessageObject->GetMessage(&stMessage);

         if(bDone)
            break;

         if(usSize == DSI_FRAMER_ERROR)
         {
            // Get the message to clear the error
            usSize = pclMessageObject->GetMessage(&stMessage, MESG_MAX_SIZE_VALUE);
            continue;
         }

         if(usSize != DSI_FRAMER_ERROR && usSize != DSI_FRAMER_TIMEDOUT && usSize != 0)
         {
            ProcessMessage(stMessage, usSize);
         }
      }
   }

   DSIThread_MutexLock(&mutexTestDone);
   UCHAR ucCondResult = DSIThread_CondSignal(&condTestDone);
   assert(ucCondResult == DSI_THREAD_ENONE);
   DSIThread_MutexUnlock(&mutexTestDone);

}


void Device::ProcessMessage(ANT_MESSAGE stMessage, USHORT usSize_)
{
    // To store the messages received/ messages going to send
   QString text;
   QString subtext;
   BOOL bStatus;
   BOOL bPrintBuffer = FALSE;
   UCHAR ucDataOffset = MESSAGE_BUFFER_DATA2_INDEX;   // For most data messages

   switch(stMessage.ucMessageID)
   {
      //RESPONSE MESG
      case MESG_RESPONSE_EVENT_ID:
      {
         //RESPONSE TYPE
         switch(stMessage.aucData[1])
         {
            case MESG_NETWORK_KEY_ID:
            {
               if(stMessage.aucData[2] != RESPONSE_NO_ERROR)
               {
                  text.sprintf("Error configuring network key: Code 0%d\n", stMessage.aucData[2]);
                  break;
               }
               text.sprintf("Network key set.\n");
               text.append("Assigning channel...\n");
               if(ucChannelType == CHANNEL_TYPE_MASTER)
               {
                  bStatus = pclMessageObject->AssignChannel(USER_ANTCHANNEL, PARAMETER_TX_NOT_RX, 0, MESSAGE_TIMEOUT);
               }
               else if(ucChannelType == CHANNEL_TYPE_SLAVE)
               {

                  bStatus = pclMessageObject->AssignChannel(USER_ANTCHANNEL, 0, 0, MESSAGE_TIMEOUT);

               }else if(ucChannelType == CHANNEL_TYPE_MULTI){

                   for(int i = 0; i< MAX_CHANNEL_NUM;i++){
                        bStatus = pclMessageObject->AssignChannel(i, 0, 0, MESSAGE_TIMEOUT);
                        text.append(subtext.sprintf("assign channel [%d] result %d\n",i,bStatus));
                   }

               }
               break;
            }

            case MESG_ASSIGN_CHANNEL_ID:
            {
               if(stMessage.aucData[2] != RESPONSE_NO_ERROR)
               {
                  text.sprintf("Error assigning channel: Code 0%d\n", stMessage.aucData[2]);
                  break;
               }
               //text.append("Channel assigned\n");

               bStatus = pclMessageObject->SetChannelID(channel_to_setID,ucDeviceNum,ucDeviceType,ucTransType, MESSAGE_TIMEOUT);
               text.append(subtext.sprintf("set channel [%d] ID result %d\n",channel_to_setID,bStatus));
               channel_to_setID++;
               break;

            }

            case MESG_CHANNEL_ID_ID:
            {
               if(stMessage.aucData[2] != RESPONSE_NO_ERROR)
               {
                  text.sprintf("Error configuring Channel ID: Code 0%d\n", stMessage.aucData[2]);
                  break;
               }
              // text.append(subtext.sprintf("Channel ID set [%d]\n",channel_to_setID));
               UCHAR ucRF =  RF_INDEX+10*channel_to_setRF;
               bStatus = pclMessageObject->SetChannelRFFrequency(channel_to_setRF,ucRF, MESSAGE_TIMEOUT);
               text.append(subtext.sprintf("Setting Radio Frequency channel [%d] RF %d result %d \n",channel_to_setRF,2400+ucRF,bStatus));
               channel_to_setRF++;
               break;
            }

            case MESG_CHANNEL_RADIO_FREQ_ID:
            {
               if(stMessage.aucData[2] != RESPONSE_NO_ERROR)
               {
                  text.append(subtext.sprintf("Error configuring Radio Frequency: Code 0%d\n", stMessage.aucData[2]));
                  break;
               }
               //text.append("Radio Frequency set\n");

               bBroadcasting = TRUE;

               bStatus = pclMessageObject->OpenChannel(channel_to_open, MESSAGE_TIMEOUT);
               text.append(subtext.sprintf("Opening channel [%d] result: %d",channel_to_open,bStatus));
               channel_to_open++;
               break;
            }

            case MESG_OPEN_CHANNEL_ID:
            {
               if(stMessage.aucData[2] != RESPONSE_NO_ERROR)
               {

                  text.append(subtext.sprintf("Error opening channel: Code 0%d\n", stMessage.aucData[2]));
                  bBroadcasting = FALSE;
                  break;
               }
             //  text.append("Chanel opened\n");
#if defined (ENABLE_EXTENDED_MESSAGES)
               text.append("Enabling extended messages...\n");
               pclMessageObject->RxExtMesgsEnable(TRUE);
#endif
               break;
            }

            case MESG_RX_EXT_MESGS_ENABLE_ID:
            {
               if(stMessage.aucData[2] == INVALID_MESSAGE)
               {
                  text.append("Extended messages not supported in this ANT product\n");
                  break;
               }
               else if(stMessage.aucData[2] != RESPONSE_NO_ERROR)
               {
                  QString subtext;
                  text.append(subtext.sprintf("Error enabling extended messages: Code 0%d\n", stMessage.aucData[2]));
                  break;
               }
               text.append("Extended messages enabled\n");
               break;
            }

            case MESG_UNASSIGN_CHANNEL_ID:
            {
               if(stMessage.aucData[2] != RESPONSE_NO_ERROR)
               {

                  text.append(subtext.sprintf("Error unassigning channel: Code 0%d\n", stMessage.aucData[2]));
                  break;
               }
               text.append("Channel unassigned\n");
               text.append("Press enter to exit\n");
               bMyDone = TRUE;
               break;
            }

            case MESG_CLOSE_CHANNEL_ID:
            {
               if(stMessage.aucData[2] == CHANNEL_IN_WRONG_STATE)
               {
                  // We get here if we tried to close the channel after the search timeout (slave)
                  text.append("Channel is already closed\n");
                  text.append("Unassigning channel...\n");
                  bStatus = pclMessageObject->UnAssignChannel(USER_ANTCHANNEL, MESSAGE_TIMEOUT);
                  break;
               }
               else if(stMessage.aucData[2] != RESPONSE_NO_ERROR)
               {
                  text.append(subtext.sprintf("Error closing channel: Code 0%d\n", stMessage.aucData[2]));
                  break;
               }
               // If this message was successful, wait for EVENT_CHANNEL_CLOSED to confirm channel is closed
               break;
            }

            case MESG_REQUEST_ID:
            {
               if(stMessage.aucData[2] == INVALID_MESSAGE)
               {
                  text.append("Requested message not supported in this ANT product\n");
               }
               break;
            }

            case MESG_EVENT_ID:
            {
               switch(stMessage.aucData[2])
                  {
                  case EVENT_CHANNEL_CLOSED:
                  {
                     text.append("Channel Closed\n");
                     text.append("Unassigning channel...\n");
                     bStatus = pclMessageObject->UnAssignChannel(USER_ANTCHANNEL, MESSAGE_TIMEOUT);
                     break;
                  }
                  case EVENT_TX:
                  {
                     // This event indicates that a message has just been
                     // sent over the air. We take advantage of this event to set
                     // up the data for the next message period.
                     static UCHAR ucIncrement = 0;      // Increment the first byte of the buffer

                     aucTransmitBuffer[0] = 0x00;
                     aucTransmitBuffer[1] = 0x01;
                     aucTransmitBuffer[2] = 0x02;
                     aucTransmitBuffer[3] = 0x03;
                     aucTransmitBuffer[4] = 0x04;
                     aucTransmitBuffer[5] = 0x05;
                     aucTransmitBuffer[6] = 0x06;
                     aucTransmitBuffer[7] = 0x07;

                     // Broadcast data will be sent over the air on
                     // the next message period.
                     if(bBroadcasting)
                     {
                        pclMessageObject->SendBroadcastData(USER_ANTCHANNEL, aucTransmitBuffer);

                        // Echo what the data will be over the air on the next message period.
                        if(bDisplay)
                        {


                           text.sprintf("Tx:(%d): [%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x]\n",
                              USER_ANTCHANNEL,
                              aucTransmitBuffer[MESSAGE_BUFFER_DATA1_INDEX],
                              aucTransmitBuffer[MESSAGE_BUFFER_DATA2_INDEX],
                              aucTransmitBuffer[MESSAGE_BUFFER_DATA3_INDEX],
                              aucTransmitBuffer[MESSAGE_BUFFER_DATA4_INDEX],
                              aucTransmitBuffer[MESSAGE_BUFFER_DATA5_INDEX],
                              aucTransmitBuffer[MESSAGE_BUFFER_DATA6_INDEX],
                              aucTransmitBuffer[MESSAGE_BUFFER_DATA7_INDEX],
                              aucTransmitBuffer[MESSAGE_BUFFER_DATA8_INDEX]);
                        }
                        else
                        {
                           static int iIndex = 0;
                           static char ac[] = {'|','/','-','\\'};

                           text.append(subtext.sprintf("Tx: %c\r",ac[iIndex++]));
                           iIndex &= 3;
                        }
                     }
                     break;

                  }
                  case EVENT_RX_SEARCH_TIMEOUT:
                  {
                     text.append("Search Timeout\n");
                     break;
                  }
                  case EVENT_RX_FAIL:
                  {
                     text.append("Rx Fail\n");
                     break;
                  }
                  case EVENT_TRANSFER_RX_FAILED:
                  {
                     text.append("Burst receive has failed\n");
                     break;
                  }
                  case EVENT_TRANSFER_TX_COMPLETED:
                  {
                     text.append("Tranfer Completed\n");
                     break;
                  }
                  case EVENT_TRANSFER_TX_FAILED:
                  {
                     text.append("Tranfer Failed\n");
                     break;
                  }
                  case EVENT_RX_FAIL_GO_TO_SEARCH:
                  {
                    beacon_exist = FALSE;
                    UCHAR uCh =stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX];
                    text.append(subtext.sprintf("Channel %d searching new beacon....\n",uCh));
                    emit goToSearch(uCh);
                    break;
                  }
                  case EVENT_CHANNEL_COLLISION:
                  {
                     text.append("Channel Collision\n");
                     break;
                  }
                  case EVENT_TRANSFER_TX_START:
                  {
                     text.append("Burst Started\n");
                     break;
                  }
                  default:
                  {

                     text.append(subtext.sprintf("Unhandled channel event: 0x%X\n", stMessage.aucData[2]));
                     break;
                  }

               }

               break;
            }
            case MESG_ANTLIB_CONFIG_ID:
            {
                text + "ANT lib configure finish\n";
                break;
            }
            default:
            {

               text.append(subtext.sprintf("Unhandled response 0%d to message 0x%X\n", stMessage.aucData[2], stMessage.aucData[1]));
               break;
            }
         }
         break;
      }

      case MESG_STARTUP_MESG_ID:
      {
         text.append("RESET Complete, reason: ");

         UCHAR ucReason = stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX];

         if(ucReason == RESET_POR)
            text.append("RESET_POR");
         if(ucReason & RESET_SUSPEND)
            text.append("RESET_SUSPEND ");
         if(ucReason & RESET_SYNC)
            text.append("RESET_SYNC ");
         if(ucReason & RESET_CMD)
            text.append("RESET_CMD ");
         if(ucReason & RESET_WDT)
            text.append("RESET_WDT ");
         if(ucReason & RESET_RST)
            text.append("RESET_RST ");
         text.append("\n");

         break;
      }

      case MESG_CAPABILITIES_ID:
      {
         text.append("CAPABILITIES:\n");

         text.append(subtext.sprintf("   Max ANT Channels: %d\n",stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX]));
         text.append(subtext.sprintf("   Max ANT Networks: %d\n",stMessage.aucData[MESSAGE_BUFFER_DATA2_INDEX]));

         UCHAR ucStandardOptions = stMessage.aucData[MESSAGE_BUFFER_DATA3_INDEX];
         UCHAR ucAdvanced = stMessage.aucData[MESSAGE_BUFFER_DATA4_INDEX];
         UCHAR ucAdvanced2 = stMessage.aucData[MESSAGE_BUFFER_DATA5_INDEX];

         text.append("Standard Options:\n");
         if( ucStandardOptions & CAPABILITIES_NO_RX_CHANNELS )
            text.append("CAPABILITIES_NO_RX_CHANNELS\n");
         if( ucStandardOptions & CAPABILITIES_NO_TX_CHANNELS )
            text.append("CAPABILITIES_NO_TX_CHANNELS\n");
         if( ucStandardOptions & CAPABILITIES_NO_RX_MESSAGES )
            text.append("CAPABILITIES_NO_RX_MESSAGES\n");
         if( ucStandardOptions & CAPABILITIES_NO_TX_MESSAGES )
            text.append("CAPABILITIES_NO_TX_MESSAGES\n");
         if( ucStandardOptions & CAPABILITIES_NO_ACKD_MESSAGES )
            text.append("CAPABILITIES_NO_ACKD_MESSAGES\n");
         if( ucStandardOptions & CAPABILITIES_NO_BURST_TRANSFER )
            text.append("CAPABILITIES_NO_BURST_TRANSFER\n");

         text.append("Advanced Options:\n");
         if( ucAdvanced & CAPABILITIES_OVERUN_UNDERRUN )
            text.append("CAPABILITIES_OVERUN_UNDERRUN\n");
         if( ucAdvanced & CAPABILITIES_NETWORK_ENABLED )
            text.append("CAPABILITIES_NETWORK_ENABLED\n");
         if( ucAdvanced & CAPABILITIES_AP1_VERSION_2 )
            text.append("CAPABILITIES_AP1_VERSION_2\n");
         if( ucAdvanced & CAPABILITIES_SERIAL_NUMBER_ENABLED )
            text.append("CAPABILITIES_SERIAL_NUMBER_ENABLED\n");
         if( ucAdvanced & CAPABILITIES_PER_CHANNEL_TX_POWER_ENABLED )
            text.append("CAPABILITIES_PER_CHANNEL_TX_POWER_ENABLED\n");
         if( ucAdvanced & CAPABILITIES_LOW_PRIORITY_SEARCH_ENABLED )
            text.append("CAPABILITIES_LOW_PRIORITY_SEARCH_ENABLED\n");
         if( ucAdvanced & CAPABILITIES_SCRIPT_ENABLED )
            text.append("CAPABILITIES_SCRIPT_ENABLED\n");
         if( ucAdvanced & CAPABILITIES_SEARCH_LIST_ENABLED )
            text.append("CAPABILITIES_SEARCH_LIST_ENABLED\n");

         if(usSize_ > 4)
         {
            text.append("Advanced 2 Options 1:\n");
            if( ucAdvanced2 & CAPABILITIES_LED_ENABLED )
               text.append("CAPABILITIES_LED_ENABLED\n");
            if( ucAdvanced2 & CAPABILITIES_EXT_MESSAGE_ENABLED )
               text.append("CAPABILITIES_EXT_MESSAGE_ENABLED\n");
            if( ucAdvanced2 & CAPABILITIES_SCAN_MODE_ENABLED )
               text.append("CAPABILITIES_SCAN_MODE_ENABLED\n");
            if( ucAdvanced2 & CAPABILITIES_RESERVED )
               text.append("CAPABILITIES_RESERVED\n");
            if( ucAdvanced2 & CAPABILITIES_PROX_SEARCH_ENABLED )
               text.append("CAPABILITIES_PROX_SEARCH_ENABLED\n");
            if( ucAdvanced2 & CAPABILITIES_EXT_ASSIGN_ENABLED )
               text.append("CAPABILITIES_EXT_ASSIGN_ENABLED\n");
            if( ucAdvanced2 & CAPABILITIES_FS_ANTFS_ENABLED)
               text.append("CAPABILITIES_FREE_1\n");
            if( ucAdvanced2 & CAPABILITIES_FIT1_ENABLED )
               text.append("CAPABILITIES_FIT1_ENABLED\n");
         }
         break;
      }
      case MESG_CHANNEL_STATUS_ID:
      {
         text.append("Got Status\n");

         char astrStatus[][32] = {  "STATUS_UNASSIGNED_CHANNEL",
                                    "STATUS_ASSIGNED_CHANNEL",
                                    "STATUS_SEARCHING_CHANNEL",
                                    "STATUS_TRACKING_CHANNEL"   };

         UCHAR ucStatusByte = stMessage.aucData[MESSAGE_BUFFER_DATA2_INDEX] & STATUS_CHANNEL_STATE_MASK; // MUST MASK OFF THE RESERVED BITS
         text.append(subtext.sprintf("STATUS: %s\n",astrStatus[ucStatusByte]));
         break;
      }
      case MESG_CHANNEL_ID_ID:
      {
         // Channel ID of the device that we just recieved a message from.
         USHORT usDeviceNumber = stMessage.aucData[MESSAGE_BUFFER_DATA2_INDEX] | (stMessage.aucData[MESSAGE_BUFFER_DATA3_INDEX] << 8);
         UCHAR ucDeviceType =  stMessage.aucData[MESSAGE_BUFFER_DATA4_INDEX];
         UCHAR ucTransmissionType = stMessage.aucData[MESSAGE_BUFFER_DATA5_INDEX];


         text.append(subtext.sprintf("CHANNEL ID: (%d/%d/%d)\n", usDeviceNumber, ucDeviceType, ucTransmissionType));
         break;
      }
      case MESG_VERSION_ID:
      {
         text.append(subtext.sprintf("VERSION: %s\n", (char*) &stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX]));
         break;
      }
      case MESG_ACKNOWLEDGED_DATA_ID:
      case MESG_BURST_DATA_ID:
      case MESG_BROADCAST_DATA_ID:
      {
       if(beacon_exist == FALSE){

           UCHAR uCh = stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX];
           qDebug()<<"++++new beacon in channel "<<uCh;
           emit newBeacon(static_cast<int>(uCh));
       }
       beacon_exist = TRUE;
         // The flagged and unflagged data messages have the same
         // message ID. Therefore, we need to check the size to
         // verify of a flag is present at the end of a message.
         // To enable flagged messages, must call ANT_RxExtMesgsEnable first.
         if(usSize_ > MESG_DATA_SIZE)
         {
            UCHAR ucFlag = stMessage.aucData[MESSAGE_BUFFER_DATA10_INDEX];

            if(bDisplay && ucFlag & ANT_EXT_MESG_BITFIELD_DEVICE_ID)
            {
               // Channel ID of the device that we just recieved a message from.
               USHORT usDeviceNumber = stMessage.aucData[MESSAGE_BUFFER_DATA11_INDEX] | (stMessage.aucData[MESSAGE_BUFFER_DATA12_INDEX] << 8);
               UCHAR ucDeviceType =  stMessage.aucData[MESSAGE_BUFFER_DATA13_INDEX];
               UCHAR ucTransmissionType = stMessage.aucData[MESSAGE_BUFFER_DATA14_INDEX];

               text.append(subtext.sprintf("Chan ID(%d/%d/%d) - ", usDeviceNumber, ucDeviceType, ucTransmissionType));
            }
         }

         // Display recieved message
         bPrintBuffer = TRUE;
         ucDataOffset = MESSAGE_BUFFER_DATA2_INDEX;   // For most data messages

         if(bDisplay)
         {
            if(stMessage.ucMessageID == MESG_ACKNOWLEDGED_DATA_ID )
               text.append(subtext.sprintf("Acked Rx:(%d): ", stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX]));
            else if(stMessage.ucMessageID == MESG_BURST_DATA_ID)
               text.append(subtext.sprintf("Burst(0x%02x) Rx:(%d): ", ((stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX] & 0xE0) >> 5), stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX] & 0x1F) );
            else
               text.append(subtext.sprintf("Rx:(%d): ", stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX]));
         }
         break;
      }
      case MESG_EXT_BROADCAST_DATA_ID:
      case MESG_EXT_ACKNOWLEDGED_DATA_ID:
      case MESG_EXT_BURST_DATA_ID:
      {

         // The "extended" part of this message is the 4-byte channel
         // id of the device that we recieved this message from. This message
         // is only available on the AT3. The AP2 uses flagged versions of the
         // data messages as shown above.

         // Channel ID of the device that we just recieved a message from.
         USHORT usDeviceNumber = stMessage.aucData[MESSAGE_BUFFER_DATA2_INDEX] | (stMessage.aucData[MESSAGE_BUFFER_DATA3_INDEX] << 8);
         UCHAR ucDeviceType =  stMessage.aucData[MESSAGE_BUFFER_DATA4_INDEX];
         UCHAR ucTransmissionType = stMessage.aucData[MESSAGE_BUFFER_DATA5_INDEX];

         bPrintBuffer = TRUE;
         ucDataOffset = MESSAGE_BUFFER_DATA6_INDEX;   // For most data messages

         if(bDisplay)
         {
            // Display the channel id
            text.append(subtext.sprintf("Chan ID(%d/%d/%d) ", usDeviceNumber, ucDeviceType, ucTransmissionType) );

            if(stMessage.ucMessageID == MESG_EXT_ACKNOWLEDGED_DATA_ID)
               text.append(subtext.sprintf("- Acked Rx:(%d): ", stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX]));
            else if(stMessage.ucMessageID == MESG_EXT_BURST_DATA_ID)
               text.append(subtext.sprintf("- Burst(0x%02x) Rx:(%d): ", ((stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX] & 0xE0) >> 5), stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX] & 0x1F) );
            else
               text.append(subtext.sprintf("- Rx:(%d): ",stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX]));
         }

         break;
      }

      default:
      {
         break;
      }
   }

   // If we recieved a data message, diplay its contents here.
   if(bPrintBuffer)
   {
      if(bDisplay)
      {
         QString content;
         content.sprintf("[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x],[%02x]\n",
            stMessage.aucData[ucDataOffset + 0],
            stMessage.aucData[ucDataOffset + 1],
            stMessage.aucData[ucDataOffset + 2],
            stMessage.aucData[ucDataOffset + 3],
            stMessage.aucData[ucDataOffset + 4],
            stMessage.aucData[ucDataOffset + 5],
            stMessage.aucData[ucDataOffset + 6],
            stMessage.aucData[ucDataOffset + 7],
            stMessage.aucData[ucDataOffset + 8],
            stMessage.aucData[ucDataOffset + 9],
            stMessage.aucData[ucDataOffset + 10],
            stMessage.aucData[ucDataOffset + 11],
            stMessage.aucData[ucDataOffset + 12],
            stMessage.aucData[ucDataOffset + 13],
            stMessage.aucData[ucDataOffset + 14],
            stMessage.aucData[ucDataOffset + 15]);
         text.append(content);

         QString serialNum;
         int ch,iRSSI;
         ch= stMessage.aucData[MESSAGE_BUFFER_DATA1_INDEX];

         serialNum.sprintf("%02X",stMessage.aucData[ucDataOffset + 6]);

         UCHAR ucRSSI_comp = stMessage.aucData[ucDataOffset + 14];
         if(ucRSSI_comp > (UCHAR)0x80){
             UCHAR ucRSSI_origin = ~(ucRSSI_comp-1);
             iRSSI = static_cast<int>(ucRSSI_origin)*(-1);
         }else{
             iRSSI = static_cast<int>(ucRSSI_comp);
         }
        qDebug()<<" ch= "<<ch<<" serialNum = "<<serialNum<<"iRSSI = "<<iRSSI;
         emit  beaconInfo(ch,serialNum,iRSSI);

      }
      else
      {
         static int iIndex = 0;
         static char ac[] = {'|','/','-','\\'};
         text.append(subtext.sprintf("Rx: %c\r",ac[iIndex++]));
         iIndex &= 3;

      }
   }
   if(!text.isEmpty()){
       new_text(text);
       qDebug()<<"process message: \n"<<text;
   }
   return;
}





// initalize ANT node and get ANT node info
BOOL Device::connectDevice(){


    BOOL ret = Init(ucUSBDeviceNum,ucChannelType);
    if(ret){
        isConnect = TRUE;
    }else{
        qDebug()<<"Device::connectDevice() failed: Device Init() failed";
        return FALSE;
    }

    // Retrieve info
    pclMessageObject->GetDeviceUSBVID(usDeviceVID);
    pclMessageObject->GetDeviceUSBPID(usDevicePID);
    pclMessageObject->GetDeviceUSBInfo(pclSerialObject->GetDeviceNumber(), aucDeviceDescription, aucDeviceSerial, USB_MAX_STRLEN);

    return true;
}

//configure ANT node
BOOL Device::configure(){

    BOOL bStatus;

    // Reset system
    qDebug("Resetting module...\n");
    bStatus = pclMessageObject->ResetSystem();
    DSIThread_Sleep(1000);
    if(!bStatus){
           qDebug()<<"Device::config() ResetSystem failed";
           return FALSE;
    }
    // Start the test by setting network key
    qDebug("Setting network key...\n");
    UCHAR ucNetKey[8] = USER_NETWORK_KEY;
    bStatus = pclMessageObject->SetNetworkKey(USER_NETWORK_NUM, ucNetKey, MESSAGE_TIMEOUT);
    if(!bStatus){
           qDebug()<<"Device::config() set network key failed";
            return FALSE;
    }

    //Enable RSSI measurement
    bStatus = pclMessageObject->SetLibConfig(ANT_LIB_CONFIG_MESG_OUT_INC_RSSI);
    if(!bStatus){
           qDebug()<<"Device::config() set lib config failed";
           return FALSE;
    }

    return TRUE;

}

//close channel and ANT node
void Device::disconnect(){
    Close();
    channel_to_open = 0;
    channel_to_setID = 0;
    channel_to_setRF = 0;
}


void Device::sendAMessageAndStoreInBuffer(QString qstr){
    UCHAR ucArr[ANT_STANDARD_DATA_PAYLOAD_SIZE];
    for(int i = 0; i < ANT_STANDARD_DATA_PAYLOAD_SIZE;i++){
        ucArr[i] = qstr.section('-',i,i).toInt(0,16);
    }
    memcpy(aucTransmitBuffer,ucArr, ANT_STANDARD_DATA_PAYLOAD_SIZE);
    BOOL ret = pclMessageObject->SendBroadcastData(USER_ANTCHANNEL, aucTransmitBuffer);
    qDebug()<<" send and store message in transmit buffer result: "<<ret;
}


