#ifndef NRF24_FUNCTIONS_H
#define NRF24_FUNCTIONS_H

#include <stdint.h>

void nrf24_rx_tx_init(uint8_t* addr, uint8_t tx_enabled);
void nrf24_scan_init(void);
void nrf24_scan_channel_signal(uint8_t data[], uint8_t len, uint8_t start, uint8_t last, uint8_t scans);
void nrf24_scan_step(void);

#endif /* NRF24_FUNCTIONS_H */