#include "adc_framework_thd.h"

/* adc_framework_thd entry function */
static uint16_t adc_water_level;
static uint16_t adc_brightness;

void g_adc_framework_user_callback(sf_adc_periodic_callback_args_t * p_args){
    uint32_t index=p_args->buffer_index;

    adc_brightness=(uint16_t)(g_user_buffer[index]+g_user_buffer[index+2]+g_user_buffer[index+4])/3;
    adc_water_level=(uint16_t)(g_user_buffer[index+1]+g_user_buffer[index+3]+g_user_buffer[index+5])/3;

}
void adc_framework_thd_entry(void)
{
    /* TODO: add your own code here */
    ssp_err_t err;

    err=g_sf_adc_periodic0.p_api->start(g_sf_adc_periodic0.p_ctrl);
    if(err!=SSP_SUCCESS)for(;;);
    while (1)
    {
        tx_thread_sleep (20);
    }
}
