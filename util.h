#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define bit(bit) (1u << bit)

void util_init(void);
void delay_us(uint16_t delay);
void normalize(uint8_t *data, uint8_t len, uint8_t range);
void plot_data(uint8_t *data, uint8_t len, const uint8_t* symbols, uint8_t symbols_len);
void print_table_header(void);


#endif /* GEN_DOC_UTIL_H_ */