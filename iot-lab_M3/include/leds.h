#ifndef LEDS_H_
#define LEDS_H_

void leds_init(void);

#define LED_RED_PIN         (1<<2)
#define LED_GREEN_PIN       (1<<5)
#define LED_ORANGE_PIN      (1<<10)

#define LED_RED_PORT        (GPIOD->ODR)
#define LED_GREEN_PORT      (GPIOB->ODR)
#define LED_ORANGE_PORT     (GPIOC->ODR)

#define LED_RED_ON          (LED_RED_PORT &= ~LED_RED_PIN)
#define LED_RED_OFF         (LED_RED_PORT |= LED_RED_PIN)
#define LED_RED_TOGGLE      (LED_RED_PORT ^= LED_RED_PIN)

#define LED_GREEN_ON        (LED_GREEN_PORT &= ~LED_GREEN_PIN)
#define LED_GREEN_OFF       (LED_GREEN_PORT |= LED_GREEN_PIN)
#define LED_GREEN_TOGGLE    (LED_GREEN_PORT ^= LED_GREEN_PIN)

#define LED_ORANGE_ON       (LED_ORANGE_PORT &= ~LED_ORANGE_PIN)
#define LED_ORANGE_OFF      (LED_ORANGE_PORT |= LED_ORANGE_PIN)
#define LED_ORANGE_TOGGLE   (LED_ORANGE_PORT ^= LED_ORANGE_PIN)

#endif
