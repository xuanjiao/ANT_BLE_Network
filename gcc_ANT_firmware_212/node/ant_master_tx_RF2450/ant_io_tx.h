#ifndef ANT_IO_TX__
#define ANT_IO_TX_H__

#include <stdint.h>
#include "bsp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Function for configuring and opening master channel.
 *
 */
void ant_io_tx_setup(void);

#ifdef __cplusplus
}
#endif

#endif  // ANT_IO_TX_H__
