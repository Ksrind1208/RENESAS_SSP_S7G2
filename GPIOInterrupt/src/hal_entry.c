#include "hal_data.h"
// Biến theo dõi trạng thái LED
#define BUTTON_PRESSED IOPORT_LEVEL_LOW
#define LED_ON 1
#define LED_OFF 0
static volatile bool led_status=false;
void g_external_irq11_callback(external_irq_callback_args_t * p_args)
{
 ioport_level_t p_pin_value;
 led_status=!led_status;
 g_ioport.p_api->pinRead(IOPORT_PORT_00_PIN_06, &p_pin_value);
 g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, p_pin_value == BUTTON_PRESSED ? LED_ON : LED_OFF);
}

void hal_entry(void)
{
    g_external_irq11.p_api->open(g_external_irq11.p_ctrl, g_external_irq11.p_cfg);
    g_external_irq11.p_api->enable(g_external_irq11.p_ctrl);
    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, LED_OFF);

    while (1)
    {


    }
}
