#include "main.h"
#include "util.h"
#include <stdio.h>

extern TIM_HandleTypeDef htim2;

/**
 * @brief Initialize the utility functions.
 */
void util_init(void)
{
    HAL_TIM_Base_Start(&htim2);
}

/**
 * @brief Delay in microseconds.
 * @param ui16_delay Delay in microseconds.
 */
void delay_us(uint16_t ui16_delay)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0u);
    while (__HAL_TIM_GET_COUNTER(&htim2) < ui16_delay)
        ;
}

/**
 * @brief Normalize data to a given range.
 * @param data Data to normalize.
 * @param len Length of the data.
 * @param range Range to normalize to.
 */
void normalize(uint8_t *data, uint8_t len, uint8_t range)
{
    if(len == 0 || data == NULL || range == 0)
    {
        return;
    }
    uint8_t ui8_max = data[0];
    for (uint8_t i = 1; i < len; i++)
    {
        if (data[i] > ui8_max)
        {
            ui8_max = data[i];
        }
    }
    for (uint8_t i = 0; i < len; i++)
    {
        data[i] = data[i] * range / ui8_max;
    }
}

/**
 * @brief Plot data using symbols.
 * @param data Data to plot.
 * @param len Length of the data.
 * @param symbols Symbols to use for plotting.
 * @param symbols_len Length of the symbols.
 */
void plot_data(uint8_t *data, uint8_t len, const uint8_t *symbols, uint8_t symbols_len)
{
    if(len == 0 || data == NULL || symbols == NULL || symbols_len == 0)
    {
        return;
    }
    normalize(data, len, symbols_len - 1);
    for (uint8_t i = 0; i < len; i++)
    {
        data[i] = symbols[data[i]];
    }
}

/**
 * @brief Print table header.
 */
void print_table_header(void)
{
    printf("\n\n");
    for(int i = 0; i < 128; i++){
      printf("_");
    }
    printf("\n|");
    for(uint8_t i = 0; i < 126; i++){
      printf("%01X", i/16);
    }
    printf("|\n|");
    for(uint8_t i = 0; i < 126; i++){
      printf("%01X", i%16);
    }
    printf("|\n|");
    for(int i = 0; i < 126; i++){
      printf("_");
    }
    printf("|\n");
}
