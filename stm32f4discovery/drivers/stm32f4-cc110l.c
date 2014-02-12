/******************************************************************************
 Copyright 2008, Freie Universitaet Berlin (FUB). All rights reserved.

 These sources were developed at the Freie Universitaet Berlin, Computer Systems
 and Telematics group (http://cst.mi.fu-berlin.de).
 -------------------------------------------------------------------------------
 This file is part of FeuerWare.

 This program is free software: you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or (at your option) any later
 version.

 FeuerWare is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 this program.  If not, see http://www.gnu.org/licenses/ .
 --------------------------------------------------------------------------------
 For further information and questions please use the web site
 http://scatterweb.mi.fu-berlin.de
 and the mailinglist (subscription via web site)
 scatterweb@lists.spline.inf.fu-berlin.de
 *******************************************************************************/

/**
 * @file
 * @ingroup		LPC2387
 * @brief		CC1100 LPC2387 dependend functions
 *
 * @author      Freie Universität Berlin, Computer Systems & Telematics, FeuerWhere project
 * @author		Heiko Will <hwill@inf.fu-berlin.de>
 * @author		Thomas Hillebrandt <hillebra@inf.fu-berlin.de>
 * @version     $Revision: 1781 $
 *
 * @note    	$Id: msba2acc-cc1100.c 1781 2010-01-26 13:39:36Z hillebra $
 */

#include <stdio.h>
#include <stddef.h>
// cpu
#include "cpu.h"
// sys
#include "cc1100.h"
#include "arch_cc1100.h"
#include "cc1100_spi.h"
//#include "gpioint.h"

#define CC1100_GDO0         (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) )	// read serial I/O (GDO0)
#define CC1100_GDO1         (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6 ))	// read serial I/O (GDO1)
#define CC1100_GDO2         (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) )	// read serial I/O (GDO2)
#define SPI_TX_EMPTY				(SSP1SR & SSPSR_TFE)
#define SPI_BUSY					(SSP1SR & SSPSR_BSY)
#define SPI_RX_AVAIL				(SSP1SR & SSPSR_RNE)

#define CC1100_GDO1_LOW_RETRY		 (100)		// max. retries for GDO1 to go low
#define CC1100_GDO1_LOW_COUNT		(2700)		// loop count (timeout ~ 500 us) to wait
// for GDO1 to go low when CS low

//#define DEBUG
#ifdef DEBUG

#include "stdio.h"

static unsigned long time_value;

static void set_time(void)
{
    time_value = 0;
}

static int test_time(int code)
{
    time_value++;

    if (time_value > 10000000) {
        printf("CC1100 SPI alarm: %u!\n", code);
        time_value = 0;
        return 1;
    }

    return 0;
}
#endif

int cc110x_get_gdo0(void)
{
    return CC1100_GDO0;
}

int cc110x_get_gdo1(void)
{
    return CC1100_GDO1;
}

int cc110x_get_gdo2(void)
{
    return CC1100_GDO2;
}

void cc110x_spi_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    /* GPIOD Periph clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    // configure chip-select	- PA04
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, SET);

    /* Peripheral Clock Enable -------------------------------------------------*/
    /* Enable the SPI clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /* Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* SPI GPIO Configuration --------------------------------------------------*/
    /* Connect SPI pins to AF5 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);		//SCK
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);	//MISO
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);	//MOSI

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

    /* SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* SPI  MISO pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* SPI  MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* SPI configuration -------------------------------------------------------*/
    SPI_I2S_DeInit(SPI1);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//82MHz/16 = 5,25MHz compared to former 6MHz
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPI1, &SPI_InitStructure);

    /* Enable the SPI peripheral */
    SPI_Cmd(SPI1, ENABLE);

    /* Clear RxFIFO	*/
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == SET) {
        SPI_I2S_ReceiveData(SPI1);
    }

}

uint8_t cc1100_txrx(uint8_t c)
{
    uint8_t result;
    SPI_I2S_SendData(SPI1, c);
#ifdef DEBUG
    set_time();
#endif

    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)) {
#ifdef DEBUG
        test_time(0);
#endif
    }

#ifdef DEBUG
    set_time();
#endif

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)) {
#ifdef DEBUG
        test_time(1);
#endif
    }

#ifdef DEBUG
    set_time();
#endif

    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)) {
#ifdef DEBUG
        test_time(2);
#endif
    }

    result = (uint8_t)SPI_I2S_ReceiveData(SPI1);
    return result;
}

void cc110x_spi_cs(void)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, RESET);
}

void cc110x_spi_select(void)
{
    volatile int retry_count = 0;
    volatile int abort_count;
    // Switch MISO to GDO mode input
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

cs_low:
    // CS to low
    abort_count = 0;
    cc110x_spi_cs();
    // Wait for SO to go low (voltage regulator
    // has stabilized and the crystal is running)
loop:
    asm volatile("nop");

    if (CC1100_GDO1) {
        abort_count++;

        if (abort_count > CC1100_GDO1_LOW_COUNT) {
            retry_count++;

            if (retry_count > CC1100_GDO1_LOW_RETRY) {
                puts("[CC1100 SPI] fatal error\n");
                goto final;
            }

            cc110x_spi_unselect();		// CS to high
            goto cs_low;		// try again
        }

        goto loop;
    }

final:
    // Switch MISO to SPI mode
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);	//MISO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    /* SPI MISO pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void cc110x_spi_unselect(void)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, SET);
}

void cc1100_before_send(void)
{
    // Disable GDO2 interrupt before sending packet
    cc110x_gdo2_disable();
}

void cc1100_after_send(void)
{
    // Enable GDO2 interrupt after sending packet
    cc110x_gdo2_enable();
}

void cc110x_gdo0_enable(void)
{
    //    gpioint_set(2, BIT6, GPIOINT_RISING_EDGE, &cc1100_gdo0_irq);
}

void cc110x_gdo0_disable(void)
{
    //	gpioint_set(2, BIT6, GPIOINT_DISABLE, NULL);
}

void cc110x_gdo2_disable(void)
{
    //	gpioint_set(0, BIT28, GPIOINT_DISABLE, NULL);
}

void cc110x_gdo2_enable(void)
{
    //	gpioint_set(0, BIT28, GPIOINT_FALLING_EDGE, &cc1100_gdo2_irq);
}

void cc1100_init_interrupts(void)
{
    // Enable external interrupt on low edge (for GDO2)
    //	FIO0DIR &= ~BIT28;
    cc110x_gdo2_enable();
    // Enable external interrupt on low edge (for GDO0)
    //	FIO2DIR &= ~BIT6;
}
