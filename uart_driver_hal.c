#include "uart_driver_hal.h"

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_tx;
DMA_HandleTypeDef hdma_rx;

uint8_t uart_rx_buf[RX_BUF_SIZE];
volatile uint16_t uart_rx_len = 0;

void uart_hal_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &gpio);

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&huart2);

    /* RX DMA */
    hdma_rx.Instance = DMA1_Stream5;
    hdma_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_rx.Init.Mode = DMA_NORMAL;
    HAL_DMA_Init(&hdma_rx);
    __HAL_LINKDMA(&huart2, hdmarx, hdma_rx);

    /* TX DMA */
    hdma_tx.Instance = DMA1_Stream6;
    hdma_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
    HAL_DMA_Init(&hdma_tx);
    __HAL_LINKDMA(&huart2, hdmatx, hdma_tx);

    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart2, uart_rx_buf, RX_BUF_SIZE);
}
