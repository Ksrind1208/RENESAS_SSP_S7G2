/* HAL-only entry function */
#include "hal_data.h"
#include "bsp_api.h"  // Include the BSP API for SK-S7G2
bsp_leds_t leds;
// Define the number of counts per millisecond (1 count per clock tick, clock rate is 120MHz)
// So there are 120E6 ticks per second.
// Divide by 1000 to get ticks / millisecond
#define COUNTS_PER_MILLISECOND  (120E6 / 1000)
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
void hal_entry(void)
{
    R_BSP_LedsGet(&leds);
    // Boolean to hold LED state
    bool isOn = true;

    // Variable to hold counts
    timer_size_t counts = 0;

    // Open the timer using the configured options from the configurator
    g_timer.p_api->open (g_timer.p_ctrl, g_timer.p_cfg);

    // Main Loop
    while(1)
    {
        // Turn LED
        if(isOn){
            turnalloff_led();

        }else{
            turnallon_led();
        }
        // Toggle LED State
        isOn = !isOn;

        // Wait for timer loop
        while (1)
        {
            // Get current counts
            g_timer.p_api->counterGet(g_timer.p_ctrl, &counts);

            // Check if 500ms has elapsed => This should be a helper function at some point
            // Need to look if the PBCLK settings are stored in a define somewhere...
            if (counts > (500*COUNTS_PER_MILLISECOND))
            {
                // Reset the timer to 0
                g_timer.p_api->reset(g_timer.p_ctrl);
                break;
            }
        }
    }
}
