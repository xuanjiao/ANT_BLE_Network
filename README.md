# Student project: Indoor localization based on two RF technologies: BLE and ANT (2018)

# Overview
Firmwares and a GUI software designed for evaluating two RF technologies: ANT and BLE.


# Prerequisites
-   [D52QM6 SoC modules](https://www.thisisant.com/developer/components/d52)
-   [nRF52 DK](https://www.nordicsemi.com/Software-and-tools/Development-Kits/nRF52-DK/Getting-Started)

-   [Qt](https://www.qt.io)
-   [SEGGER J-Link](https://www.segger.com/downloads/jlink/) 
-   [Keil MDK (nRF_DeviceFamily)](https://developer.nordicsemi.com/nRF5_SDK/pieces/nRF_DeviceFamilyPack/)

# Installation

`git clone https://github.com/xuanjiao/ANT_BLE_localization.git`

# Usage

## Peer-to-peer communication
The picture below is a Peer-to-peer setup. It contains three parts.
-   A beacon (D52QM6 SoC) periodically sends messages. 
-   A base station (nRF52 DK) connected to a laptop via USB. It measures the RSSI values.
-   A laptop which run the GUI software. It can not only configure the communication setting of the base station, but also receive values and store them. 

![Basic Setup](Doc/github/basic_setup.png)
To reduce the experimental variates, it is recommanded to perform the experiment in a large empty lab space without obstacles. 

## ANT multi-channel communication
In the multi-channel solution, multiple beacons broadcast with different radio frequency. The base station establishes multiple channels to listen to those beacons simultaneously. 

![advance setup](Doc/github/advance_setup.jpg)


## Fingleprinting localization
The fingleprint localization is mainly divided into two phases:
-   Offline training phase: Gathering RSSI values at known locations. Establishing a fingleprint database for mapping of RSSI and sampling positions.
-   Online localization phase: Measuring the RSSI value of multiple beacons and find the matching positions in the fingleprint database.

We implemented a simple fingerprint-based localization senario and evaluated the performace of BLE and ANT. The experiment results suggest that ANT-based-fingleprinting-localization can clearly outperform BLE. For more details please refer to the poster abstract [PDF](Doc/ANT_Poster.pdf)
or [ACM digital library](https://dl.acm.org/doi/10.1145/3274783.3275217)

## Main GUI windows
