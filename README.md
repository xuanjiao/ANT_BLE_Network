# Student project: Indoor localization based on two RF technologies: BLE and ANT

# Table of content
1.  Background
2.  Introduction 
3.  Installation
4.  Usage

# 1. Background

GPS (Global Positioning System) is widely used in outdoor environment for determing the location. However, it is not a suitable solution of indoor localization, because the wall attenuates the GPS signal from satellites. Therefore, we consider two RF technologies: ANT and BLE. 

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


# 3. Installation

## 1. Preparation

1.  Download programming tool nRFgo Studio https://www.nordicsemi.com/Software-and-tools/Development-Tools/nRFgo-Studio
2.  
## 2. Flash the firmware (softdevice and application) for nRF52 SoC 
1. Use nRFgo to flash softdevice for all nRF52 nodes. The software are located in firmware212/ANT_s212_nrf52_5.0.0.hex
2. Flash application RF2450.hex (node for channel 0)- RF2520.hex(node for channel 7)
## 3. Open GUI software in Host PC
