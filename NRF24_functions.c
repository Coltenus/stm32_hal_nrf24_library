#include "NRF24_functions.h"
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "NRF24_conf.h"
#include "NRF24_reg_addresses.h"
#include "NRF24.h"
#include "util.h"

/**
 * @brief This function is used to initialize NRF24 module for RX or TX mode
 * @param addr address of RX and TX pipe
 * @param tx_enabled if 1 then TX mode is enabled, if 0 then RX mode is enabled
 */
void nrf24_rx_tx_init(uint8_t* addr, uint8_t tx_enabled) {
	csn_high();

	HAL_Delay(5);

	ce_low();

	nrf24_init();

	if(tx_enabled){
		nrf24_stop_listen();
	}
	else{
		nrf24_listen();
	}

	nrf24_auto_ack_all(auto_ack);
	nrf24_en_ack_pld(enable);
	nrf24_en_dyn_ack(disable);
	nrf24_dpl(enable);

	nrf24_set_crc(no_crc, _2byte);

	nrf24_tx_pwr(_0dbm);
	nrf24_data_rate(_1mbps);
	nrf24_set_channel(95);
	nrf24_set_addr_width(5);
	printf("Channel: %d\n", nrf24_r_reg(RF_CH, 1));

	nrf24_set_rx_dpl(0, enable);
	nrf24_set_rx_dpl(1, disable);
	nrf24_set_rx_dpl(2, disable);
	nrf24_set_rx_dpl(3, disable);
	nrf24_set_rx_dpl(4, disable);
	nrf24_set_rx_dpl(5, disable);

	nrf24_pipe_pld_size(0, 1);

	nrf24_auto_retr_delay(4);
	nrf24_auto_retr_limit(10);

	nrf24_open_tx_pipe(addr);
	nrf24_open_rx_pipe(0, addr);
	ce_high();
}

/**
 * @brief This function is used to initialize NRF24 module for scanning
 */
void nrf24_scan_init(void)
{
	csn_high();

	HAL_Delay(5);

	ce_low();

	nrf24_init();
    nrf24_listen();
	nrf24_auto_ack_all(no_auto_ack);
	nrf24_en_ack_pld(disable);
	nrf24_en_dyn_ack(disable);
	nrf24_dpl(disable);

	nrf24_set_crc(no_crc, _2byte);

	nrf24_tx_pwr(_0dbm);
	nrf24_data_rate(_1mbps);

    uint8_t buf = nrf24_r_reg(CONFIG, 1);
    buf |= bit(0u) | bit(1u);
    nrf24_w_reg(CONFIG, &buf, 1);
	ce_high();
    delay_us(130u);
}

/**
 * @brief This function is used to scan the signal on the selected channel
 * @param data array to store the number of signals detected on each channel
 * @param len length of the data array
 * @param start start channel
 * @param last last channel
 * @param scans number of scans
 */
void nrf24_scan_channel_signal(uint8_t data[], uint8_t len, uint8_t start, uint8_t last, uint8_t scans)
{
    uint16_t i;
    uint8_t j;
    const uint16_t TSTBY2A = 130u; /* [us], standby mode --> RX mode */
    const uint16_t TDELAY_AGC = 40u; /* automatic gain control */
    /* TSTBY2A + TDELAY_AGC = delay for RPD status to set */
    memset(data, 0, len);
    ce_low();
    HAL_Delay(1);

    for (i = 0u; i < scans; i++)
    {
        for (j = start; j <= last && j < len; j++)
        {
            nrf24_set_channel(j);
            ce_high();
            delay_us(TSTBY2A + TDELAY_AGC);
            ce_low();
            if (nrf24_r_reg(RPD, 1) == bit(0u))
            {
                data[j]++;
            }
        }
    }
    ce_high();
}

/**
 * @brief This function is used to scan the signal on the selected channels and print it on the UART
 */
void nrf24_scan_step(void) {
	const uint8_t len = 126;
	const char* symbols = " .:-=+*#%@";
	uint8_t arr[len];
	nrf24_scan_channel_signal(arr, len, 0, len-1, 100);
	plot_data(arr, len, (const uint8_t*)symbols, strlen(symbols));
	printf("|%s|\n", arr);
}