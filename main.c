#include "uart_driver_hal.h"

int main(void)
{
    HAL_Init();
    uart_hal_init();

    while (1)
    {
        if (uart_rx_len)
        {
            HAL_UART_Transmit_DMA(&huart2, uart_rx_buf, uart_rx_len);
            uart_rx_len = 0;
        }
    }
}
