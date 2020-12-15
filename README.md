# RF Technologies for Indoor Localization and Positioning (2018)

# Overview
Firmwares and a GUI software designed for comparing RF technologies (BLE and ANT) for indoor localization.


# Requirements
-   [D52QM6 SoC modules](https://www.thisisant.com/developer/components/d52)
-   [nRF52 DK](https://www.nordicsemi.com/Software-and-tools/Development-Kits/nRF52-DK/Getting-Started)

-   [Qt](https://www.qt.io)
-   [SEGGER J-Link](https://www.segger.com/downloads/jlink/) 
-   [Keil MDK (nRF_DeviceFamily)](https://developer.nordicsemi.com/nRF5_SDK/pieces/nRF_DeviceFamilyPack/)

# Installation

```
git clone https://github.com/xuanjiao/ANT_BLE_localization.git
```

# Usage

## Peer-to-peer communication
The picture below is a Peer-to-peer setup. It contains three parts.
-   A beacon (D52QM6 SoC) periodically sends messages. 
-   A base station (nRF52 DK) connected to a laptop via USB. It measures the RSSI values.
-   A laptop which runs the GUI software. It can configure the base station's communication setting and receive RSSI values and store them. 

![Basic Setup](Doc/github/basic_setup.png)
It is recommended to experiment with a large empty lab space without obstacles. 

## ANT multi-channel communication
In the multi-channel solution, multiple beacons broadcast with different radio frequencies. The base station establishes multiple channels to listen to those beacons simultaneously. 

![advance setup](Doc/github/advance_setup.jpg)


## Fingleprinting localization
The fingerprint localization is mainly divided into two phases:
-   Offline training phase: Gather RSSI values at known locations and establish a fingerprint database to map RSSI and sampling positions.
-   Online localization phase: Measure the RSSI value of multiple beacons and find a matching position in the fingerprint database.

We implemented a simple fingerprint-based localization scenario and evaluated the performance of BLE and ANT. The experiment results suggest that ANT-based-fingerprinting-localization can outperform BLE. For more details, please refer to the poster abstract [PDF](Doc/ANT_Poster.pdf)
or [ACM_digital_library](https://dl.acm.org/doi/10.1145/3274783.3275217)

# Window and dialog widgets

### Main window
The user can click the ''ANT'' bottom to enter the ANT widget or click the ''BLE'' bottom to enter the BLE widget.
![main_window](Doc/github/GUI_main.png)

### ANT widget
Firstly the user should connect the nRF52 DK to the laptop via USB. Secondly, the user can configure the file setting and serial port setting and click the ''Connect'' bottom. Then the device details will be shown in the left text view. Once the base station finds the broadcasting beacon, the received messages will be printed on the right text view, and the RSSI values will be printed on the table view. Also, the user can send an eight-byte ANT message to beacons. 
![ant](Doc/github/GUI_ANT.png)

### BLE widget
Like the ANT widget, the user should first connect the nRF52 DK to the laptop via USB. Secondly, the user can enter the file name, choose a serial port, and then click the ''Connect'' button. The measured RSSI values will be printed on the text view.
![ant](Doc/github/GUI_BLE.png)