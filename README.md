# Student project: Indoor localization based on two RF technologies: BLE and ANT

# Table of content
1.  Background
2.  Introduction 
3.  Installation
4.  Usage

# 1. Background

The market of localization service is growing rapidly. The goal of the service is to identify the physical location of individuals or objects. GPS (Global Positioning System) is widely used in outdoor environment for determing the location. However, it is not a suitable solution of indoor localization, because the wall attenuates the GPS signal from satellites. 

Several alternatives have been developed over the years, including vision-based and radio fre- quency (RF)-based technologies. We report on our experiments with two RF technologies: ANT and BLE, and their performance in indoor environments.

# 1. Installation

## 1. Download programming tool ''nRFgo Studio''.

Download Link: https://www.nordicsemi.com/Software-and-tools/Development-Tools/nRFgo-Studio

## 2. Flash the firmwares.

We nRF52 DK as base station, SoC(D52QD2M6IA-A) as nodes. Then we use programming tool ''nRF52go Studio'' to flash firmwares (softdevice and application) for the base station and the nodes.

For the base station:
-   Softdevice: firmware212/base/ANT_s212_nrf52_1.0.2.hex
-   Application: firmware212/base/ant_network_processor_s212.hex

For the nodes:
-   Softdevice firmware212/node/ANT_s212_nrf52_5.0.0.hex
-   Application: Choose one file from firmware212/node/RF2450.hex (ANT channel 0)- firmware212/node/RF2520.hex (ANT channel 7)

## 3. Test the GUI software in Host PC.

The execution file is located in host_PC/release/ANT_BLE_hostPC.exe

# 2. Usage

## Experiment 1: Capture ANT RSSI value at different distances.

The following figure shows the ANT window. 
![window](./Report/images/ANT_GUI_window.png)

Step fot experiment:
1.  Turn on the ANT beacon.
2.  Click ''Connect'' bottom in GUI application.
3.   If an ANT base station is recognized, its VID and PID will show in the left text view. In addition, the received messages will be shown in table view and right text view. The ''count'' value should be 1. The measuring result will be stored in a txt file.
-   Txt file: application path/file/base name start distance.txt (E.g. application folder/file/myResult 20.txt)
4.   Turn off the ANT beacon. The base station will recognize its absence and the text view will print ”RX Fail” and ”Go to Search”. if reach maximal number of measurement, the application will quit.
5.  Move ANT beacon to next destance (start distance + Turn on the ANT beacon after ”Go to search” is printed. The count value will puls 1. Now the measuring result will be store in a new txt tile (E.g. application folder/file/myResult 40.txt).


# 2. Introduction
The table compare the propaties of those two RF techlonodgies
| Name | BLE| ANT|
|------|---------|--------|
| Description | BLE| ANT|
| Frequency range| 2\.400\-\-2\.483 GHz | 2\.00\-\-2\.483|
| Frequency| FHSS| Fixed frequency |
| Supported topologies| Peer\-to\-peer, star | Peer\-to\-peer star, mesh|
| Band| ISM 2\.4GHz | ISM 2\.4GHz|
| Security| 128\-bit AES| 64\-bit key, 128 \- bit AES|
| Data rate | 2 Mbps, 1 Mbps, 500 kbps, 125 kbps | 1 Mbps |
| Number of connections | Up to 20 | Very high |
| Effective throughput | Up to 1\.4 Mbps | Up to 60 kbps |
| Applications| Wearables, automation, sensors, fitness, healthcare, toys, computer peripherals, remote controls, etc\. | Wireless sensors, hubs for sports, fitness, healthcare |

