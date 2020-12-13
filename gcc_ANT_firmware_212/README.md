Hex file for node:
	Soft device: ANT_s212_nrf52_5.0.0.hex
	Application: RF2450.hex (node for channel 0)- RF2520.hex(node for channel 7)

Hex file for base:
	Soft device: ANT_s212_nrf52_1.0.2.hex
	Application: ant_network_processor_s212.hex

How to compile the node application source code and get .hex file:
	1. Open ant_master_tx_RF2450/ant_io_tx.c
	2. Modify the RF variable to target RF
		#define MY_RF     ((uint8_t)90). // change RF to 2490Mhz
	3. Run ant_master_tx_RF2450/d52_starterkit/s212/armgcc/make
	4. Hex file is in ant_master_tx_RF2450/d52_starterkit/s212/armgcc/_build