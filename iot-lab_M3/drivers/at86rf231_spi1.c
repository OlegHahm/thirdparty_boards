#include <stdio.h>
#include <stddef.h>

#include "cpu.h"
#include "at86rf231_spi1.h"

void at86rf231_spi1_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    /* RCC */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /* GPIO */

    /* Configure SPI MASTER pins */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* SPI
     * NOTE: APB2 is 72MHz, prescaler 16 => SPI @ 4.5 MHz, radio spi max is 7.5MHz
     * Clock idle low, rising edge
     */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    /* Enable interrupt */
    SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);
    /* Enable SPI */
    SPI_Cmd(SPI1, ENABLE);
}

uint8_t at86rf231_spi_transfer_byte(uint8_t byte)
{
    uint8_t ret;

    /* wait for tx buffer to be empty */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

    SPI_I2S_SendData(SPI1, byte);

    /* wait for rx buffer to be not empty */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

    ret = SPI_I2S_ReceiveData(SPI1);

    /* wait until it is not busy */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

    return ret;
}

void at86rf231_spi_transfer(const uint8_t *data_out, uint8_t *data_in, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++) {
        uint8_t ret;

        ret = at86rf231_spi_transfer_byte(data_out ? data_out[i] : 0);

        if (data_in) {
            data_in[i] = ret;
        }
    }
}

