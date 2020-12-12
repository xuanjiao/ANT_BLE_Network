#-------------------------------------------------
#
# Project created by QtCreator 2018-09-05T20:53:04
#
#-------------------------------------------------

QT       += core gui widgets
QT          += serialport

TARGET = ANT_BLE_hostPC
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    antwidget.cpp \
    Device.cpp \
    blewidget.cpp


HEADERS += \
        mainwindow.h \
    antwidget.h \
    definition.h \
    device.h \
    ANT_LIB/common/checksum.h \
    ANT_LIB/common/crc.h \
    ANT_LIB/inc/antdefines.h \
    ANT_LIB/inc/antmessage.h \
    ANT_LIB/inc/build_version.h \
    ANT_LIB/inc/defines.h \
    ANT_LIB/inc/types.h \
    ANT_LIB/inc/version.h \
    ANT_LIB/software/ANTFS/antfs_client_channel.hpp \
    ANT_LIB/software/ANTFS/antfs_client_interface.hpp \
    ANT_LIB/software/ANTFS/antfs_directory.h \
    ANT_LIB/software/ANTFS/antfs_host.hpp \
    ANT_LIB/software/ANTFS/antfs_host_channel.hpp \
    ANT_LIB/software/ANTFS/antfs_host_interface.hpp \
    ANT_LIB/software/ANTFS/antfs_interface.h \
    ANT_LIB/software/ANTFS/antfsmessage.h \
    ANT_LIB/software/ANTFS/config.h \
    ANT_LIB/software/serial/device_management/dsi_ant_device.hpp \
    ANT_LIB/software/serial/device_management/dsi_ant_device_polling.hpp \
    ANT_LIB/software/serial/device_management/dsi_ant_message_processor.hpp \
    ANT_LIB/software/serial/device_management/dsi_response_queue.hpp \
    ANT_LIB/software/serial/dsi_framer.hpp \
    ANT_LIB/software/serial/dsi_framer_ant.hpp \
    ANT_LIB/software/serial/dsi_serial.hpp \
    ANT_LIB/software/serial/dsi_serial_callback.hpp \
    ANT_LIB/software/serial/dsi_serial_generic.hpp \
    ANT_LIB/software/serial/dsi_serial_iokit.hpp \
    ANT_LIB/software/serial/dsi_vcp_si.hpp \
    ANT_LIB/software/system/dsi_convert.h \
    ANT_LIB/software/system/dsi_debug.hpp \
    ANT_LIB/software/system/dsi_thread.h \
    ANT_LIB/software/system/dsi_timer.hpp \
    ANT_LIB/software/system/dsi_ts_queue.hpp \
    ANT_LIB/software/system/macros.h \
    ANT_LIB/software/USB/device_handles/usb_device_handle.hpp \
    ANT_LIB/software/USB/device_handles/usb_device_handle_iokit.hpp \
    ANT_LIB/software/USB/device_handles/usb_device_handle_si_iokit.hpp \
    ANT_LIB/software/USB/device_handles/usb_device_handle_vcp.hpp \
    ANT_LIB/software/USB/devices/usb_device.hpp \
    ANT_LIB/software/USB/devices/usb_device_iokit.hpp \
    ANT_LIB/software/USB/devices/usb_device_vcp.hpp \
    ANT_LIB/software/USB/iokit_driver/iokit_device.hpp \
    ANT_LIB/software/USB/iokit_driver/iokit_device_handle.hpp \
    ANT_LIB/software/USB/iokit_driver/iokit_device_list.hpp \
    ANT_LIB/software/USB/iokit_driver/iokit_interface.hpp \
    ANT_LIB/software/USB/iokit_driver/iokit_transfer.hpp \
    ANT_LIB/software/USB/iokit_driver/iokit_types.hpp \
    ANT_LIB/software/USB/usb_device_list.hpp \
    ANT_LIB/software/USB/usb_device_list_template.hpp \
    ANT_LIB/software/USB/usb_standard_types.hpp \
    blewidget.h

INCLUDEPATH += ANT_LIB/common
INCLUDEPATH += ANT_LIB/inc
INCLUDEPATH += ANT_LIB/software/ANTFS
INCLUDEPATH += ANT_LIB/software/serial
INCLUDEPATH += ANT_LIB/software/system
INCLUDEPATH += ANT_LIB/software/USB
INCLUDEPATH += ANT_LIB/software/USB/devices
INCLUDEPATH += ANT_LIB/software/USB/device_handles
INCLUDEPATH += ANT_LIB/software/USB/iokit_driver
INCLUDEPATH += ANT_LIB/
INCLUDEPATH += ANT_DLL/

FORMS += \
        mainwindow.ui \
    antwidget.ui \
    blewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


macx :{
        LIBS += -L$$PWD/lib/mac/ -lant
        LIBS += -framework IOKit
        LIBS += -framework CoreFoundation
}

win32:{
    LIBS += "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17134.0\um\x86\AdvAPI32.Lib"
    LIBS += "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17134.0\um\x86\User32.Lib"
    LIBS += -L$$PWD/../ANT-SDK_PC.3.5/release/ -lANT_LIB
}
RESOURCES += \
    resource.qrc

DISTFILES +=
