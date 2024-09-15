#include "bsp_api.h"  // Include the BSP API for SK-S7G2
bsp_leds_t leds;  // Structure to hold information about the LEDs
void delay_ms(uint32_t ms) {
   // Simple delay function for blocking delay
   for (volatile uint32_t i = 0; i < (ms * 6000); i++) {
       __asm("NOP");
   }
}
void turnon_led1(){
   R_IOPORT_PinWrite(leds.p_leds[0], IOPORT_LEVEL_LOW);
   R_IOPORT_PinWrite(leds.p_leds[1], IOPORT_LEVEL_HIGH);
   R_IOPORT_PinWrite(leds.p_leds[2], IOPORT_LEVEL_HIGH);
}
void turnon_led2(){
   R_IOPORT_PinWrite(leds.p_leds[0], IOPORT_LEVEL_HIGH);
   R_IOPORT_PinWrite(leds.p_leds[1], IOPORT_LEVEL_LOW);
   R_IOPORT_PinWrite(leds.p_leds[2], IOPORT_LEVEL_HIGH);
}
void turnon_led3(){
   R_IOPORT_PinWrite(leds.p_leds[0], IOPORT_LEVEL_HIGH);
   R_IOPORT_PinWrite(leds.p_leds[1], IOPORT_LEVEL_HIGH);
   R_IOPORT_PinWrite(leds.p_leds[2], IOPORT_LEVEL_LOW);
}
void turnallon_led(){
   R_IOPORT_PinWrite(leds.p_leds[0], IOPORT_LEVEL_LOW);
   R_IOPORT_PinWrite(leds.p_leds[1], IOPORT_LEVEL_LOW);
   R_IOPORT_PinWrite(leds.p_leds[2], IOPORT_LEVEL_LOW);
}
void turnalloff_led(){
   R_IOPORT_PinWrite(leds.p_leds[0], IOPORT_LEVEL_HIGH);
   R_IOPORT_PinWrite(leds.p_leds[1], IOPORT_LEVEL_HIGH);
   R_IOPORT_PinWrite(leds.p_leds[2], IOPORT_LEVEL_HIGH);
}
void hal_entry(void) {
   // Initialize the BSP (Board Support Package)
   R_BSP_LedsGet(&leds);  // Get the LED information for the board
   // Check if there are LEDs on the board
   if (leds.led_count > 0) {
       while (1) {
           turnon_led1();
           delay_ms(500);
           turnon_led2();
           delay_ms(500);
           turnon_led3();
           delay_ms(500);
           turnalloff_led();
           delay_ms(500);
           turnallon_led();
           delay_ms(500);
           turnalloff_led();
           delay_ms(500);
           turnallon_led();
           delay_ms(500);
           turnalloff_led();
           delay_ms(500);
           turnallon_led();
           delay_ms(500);
       }
   }
}
