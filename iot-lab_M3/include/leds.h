#ifndef LEDS_H_
#define LEDS_H_

void leds_init(void);
void led_on(uint32_t);
void led_off(uint32_t);

#define LED_RED_PIN         (1<<2)
//#define LED_GREEN_PIN       (1<<5)
//
//#define LED_RED_ON()        led_on(LED_RED_PIN);
//#define LED_RED_OFF()       led_off(LED_RED_PIN);
//#define LED_RED_TOGGLE()    led_toggle(LED_RED_PIN);
//
//#define LED_GREEN_ON()      led_on(LED_GREEN_PIN);
//#define LED_GREEN_OFF()     led_off(LED_GREEN_PIN);
//#define LED_GREEN_TOGGLE()  led_toggle(LED_GREEN_PIN);

#endif
