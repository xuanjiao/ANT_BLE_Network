#ifndef DEFINITION_H
#define DEFINITION_H

#define ENABLE_EXTENDED_MESSAGES

#define USER_BAUDRATE                  (57600)  // For AT3/AP2, use 57600
#define USER_RADIOFREQ_DEFAULT         (35)

#define USER_USB_DEVICENUM_DEFAULT    (0)
#define USER_ANTCHANNEL                (0)
#define USER_DEVICENUM_DEFAULT         (49)
#define USER_DEVICETYPE_DEFAULT        (1)
#define USER_TRANSTYPE_DEFAULT         (1)

#define MAX_CHANNEL_NUM                 (8)

#define USER_CHANNELTYPE_DEFAULT       (0)
#define USER_NETWORK_KEY      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,}
#define USER_NETWORK_NUM      (0)      // The network key is assigned to this network number

#define RF_INDEX                (50)
#define MAX_CHANNEL_EVENT_SIZE   (MESG_MAX_SIZE_VALUE)     // Channel event buffer size, assumes worst case extended message size
#define MAX_RESPONSE_SIZE        (MESG_MAX_SIZE_VALUE)     // Protocol response buffer size

#define MESSAGE_TIMEOUT       (1000)

#define CHANNEL_TYPE_MASTER   (0)
#define CHANNEL_TYPE_SLAVE    (1)
#define CHANNEL_TYPE_MULTI    (2)
#define CHANNEL_TYPE_INVALID  (3)

#define COLUMN_INDEX_CHANNEL (0)
#define COLUMN_INDEX_SERIAL_NUM (1)
#define COLUMN_INDEX_RSSI (2)

#define MESSAGE_BUFFER_DATA1_INDEX ((UCHAR) 0)
#define MESSAGE_BUFFER_DATA2_INDEX ((UCHAR) 1)
#define MESSAGE_BUFFER_DATA3_INDEX ((UCHAR) 2)
#define MESSAGE_BUFFER_DATA4_INDEX ((UCHAR) 3)
#define MESSAGE_BUFFER_DATA5_INDEX ((UCHAR) 4)
#define MESSAGE_BUFFER_DATA6_INDEX ((UCHAR) 5)
#define MESSAGE_BUFFER_DATA7_INDEX ((UCHAR) 6)
#define MESSAGE_BUFFER_DATA8_INDEX ((UCHAR) 7)
#define MESSAGE_BUFFER_DATA9_INDEX ((UCHAR) 8)
#define MESSAGE_BUFFER_DATA10_INDEX ((UCHAR) 9)
#define MESSAGE_BUFFER_DATA11_INDEX ((UCHAR) 10)
#define MESSAGE_BUFFER_DATA12_INDEX ((UCHAR) 11)
#define MESSAGE_BUFFER_DATA13_INDEX ((UCHAR) 12)
#define MESSAGE_BUFFER_DATA14_INDEX ((UCHAR) 13)



#endif // DEFINITION_H
