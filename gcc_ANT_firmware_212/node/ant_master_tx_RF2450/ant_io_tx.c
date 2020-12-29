#include "ant_io_tx.h"
#include <stdint.h>
#include "string.h"
#include "ant_interface.h"
#include "ant_parameters.h"
#include "app_error.h"
#include "ant_error.h"
#include "boards.h"
#include "sdk_config.h"
#include "ant_channel_config.h"
#include "nrf_soc.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ant.h"

// Data Page Numbers
#define DIGITALIO_DATA_PID              1u                      /**< Page number: digital data. */

#define APP_ANT_OBSERVER_PRIO           1                       /**< Application's ANT observer priority. You shouldn't need to modify this value. */

#define MY_DEV_TYPE                 ((uint8_t)0x02)

#define MY_RF                       ((uint8_t)50)

// Static variables and buffers.
static uint8_t m_broadcast_data[ANT_STANDARD_DATA_PAYLOAD_SIZE];    /**< Primary data transmit buffer. */

static uint8_t count = 0;
// get message in transmit buffer, send them
// LED blinking
static void handle_transmit()
{
    uint32_t err_code;
    static uint8_t cnt = 0;
    if(cnt == 0){
        bsp_board_led_off(BSP_BOARD_LED_0);
        bsp_board_led_off(BSP_BOARD_LED_1);
        bsp_board_led_off(BSP_BOARD_LED_2);
        bsp_board_led_off(BSP_BOARD_LED_3);
        cnt = 1;
    }else{
        bsp_board_led_on(BSP_BOARD_LED_0);
        bsp_board_led_on(BSP_BOARD_LED_1);
        bsp_board_led_on(BSP_BOARD_LED_2);
        bsp_board_led_on(BSP_BOARD_LED_3);
        cnt = 0;
    }
    
    
        uint32_t device_num = (uint32_t)NRF_FICR->DEVICEID[0];
        m_broadcast_data[3] = (device_num >> 24 )& 0xFF;
        m_broadcast_data[4] = (device_num >> 16 ) & 0xFF;
        m_broadcast_data[5] = (device_num >> 8 ) & 0xFF;
        m_broadcast_data[6] = device_num & 0xFF;
    
        m_broadcast_data[7] = count;
        err_code = sd_ant_broadcast_message_tx(ANT_CHANNEL_NUM,
                                           ANT_STANDARD_DATA_PAYLOAD_SIZE,
                                          m_broadcast_data);
        APP_ERROR_CHECK(err_code);
        count++;
    if(count == 0xFF){
        count = 0x00;
    }
}

void ant_io_tx_setup(void)
{
    uint32_t err_code;

    ant_channel_config_t channel_config =
    {
        .channel_number    = ANT_CHANNEL_NUM,
        .channel_type      = CHANNEL_TYPE_MASTER ,
        .ext_assign        = 0x00,
        .rf_freq           = MY_RF,
        .transmission_type = CHAN_ID_TRANS_TYPE,
        .device_type       = MY_DEV_TYPE,
        .device_number     = NRF_FICR->DEVICEID[0],
        .channel_period    = CHAN_PERIOD,
        .network_number    = ANT_NETWORK_NUM,
    };

    // Configure channel parameters
    err_code = ant_channel_init(&channel_config);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ant_lib_config_set(ANT_LIB_CONFIG_MESG_OUT_INC_RSSI );
    APP_ERROR_CHECK(err_code);
    
    // Open channel.
    err_code = sd_ant_channel_open(ANT_CHANNEL_NUM);
    APP_ERROR_CHECK(err_code);
    

}

/**@brief Function for handling a ANT stack event.
 *
 * @param[in] p_ant_evt  ANT stack event.
 * @param[in] p_context  Context.
 */
static void ant_evt_handler(ant_evt_t * p_ant_evt, void * p_context)
{
    switch (p_ant_evt->event)
    {
            // when receive message, do nothing
        case EVENT_RX:
            
            break;

        case EVENT_TX:
            handle_transmit();
            break;

        default:
            break;
    }
}

NRF_SDH_ANT_OBSERVER(m_ant_observer, APP_ANT_OBSERVER_PRIO, ant_evt_handler, NULL);
