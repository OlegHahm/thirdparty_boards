#include <stdio.h>
#include <stddef.h>

#include "cpu.h"

#define LEDS_PORT       GPIOB

void leds_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* Configure PD0 and PD2 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void led_on(uint32_t LED_PIN)
{
  LEDS_PORT->BRR = LED_PIN;
}

void led_off(uint32_t LED_PIN)
{
  LEDS_PORT->BSRR = LED_PIN;
}

void led_toggle(uint32_t LED_PIN)
{
  LEDS_PORT->ODR ^= LED_PIN;
}
