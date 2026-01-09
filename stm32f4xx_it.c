extern UART_HandleTypeDef huart2;
extern uint8_t uart_rx_buf[];
extern volatile uint16_t uart_rx_len;

void USART2_IRQHandler(void)
{
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart2);
        uart_rx_len = RX_BUF_SIZE - __HAL_DMA_GET_COUNTER(huart2.hdmarx);
    }

    HAL_UART_IRQHandler(&huart2);
}
