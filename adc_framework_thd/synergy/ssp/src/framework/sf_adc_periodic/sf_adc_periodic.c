/***********************************************************************************************************************
 * Copyright [2015-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name    : sf_adc_periodic.c
 * Description  : ThreadX aware general periodic ADC driver functions.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "sf_adc_periodic.h"
#include "sf_adc_periodic_cfg.h"
#include "sf_adc_periodic_api.h"
#include "sf_adc_periodic_private_api.h"
#include "r_elc.h"
#include "r_transfer_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** "ADCG" in ASCII, used to identify general periodic ADC control block */
#define SF_ADC_PERIODIC_OPEN (0x41444347ULL)
/** Macro for error logger. */
#ifndef SF_ADC_PERIODIC_ERROR_RETURN
/*LDRA_INSPECTED 77 S This macro does not work when surrounded by parentheses. */
#define SF_ADC_PERIODIC_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), &g_module_name[0], &s_sf_adc_periodic_version)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void sf_adc_periodic_callback (adc_callback_args_t * p_args);
static ssp_err_t sf_adc_periodic_configure_dtc(sf_adc_periodic_instance_ctrl_t * const p_ctrl,
        sf_adc_periodic_cfg_t const * const p_cfg, adc_info_t adc_info);
#if SF_ADC_PERIODIC_CFG_PARAM_CHECKING_ENABLE
static ssp_err_t sf_adc_periodic_parameter_check(sf_adc_periodic_instance_ctrl_t * const p_ctrl,
        sf_adc_periodic_cfg_t const * const p_cfg);
#endif
static ssp_err_t sf_adc_periodic_initialize_hal(sf_adc_periodic_instance_ctrl_t * const p_ctrl,
        sf_adc_periodic_cfg_t const * const p_cfg);
static void sf_adc_periodic_close_hal(sf_adc_periodic_instance_ctrl_t * const p_ctrl);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
#if defined(__GNUC__)
/* This structure is affected by warnings from a GCC compiler bug. This pragma suppresses the warnings in this
 * structure only.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t s_sf_adc_periodic_version =
{
    .api_version_minor  = SF_ADC_PERIODIC_API_VERSION_MINOR,
    .api_version_major  = SF_ADC_PERIODIC_API_VERSION_MAJOR,
    .code_version_major = SF_ADC_PERIODIC_CODE_VERSION_MAJOR,
    .code_version_minor = SF_ADC_PERIODIC_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif
/** Name of module used by error logger macro */
#if BSP_CFG_ERROR_LOG != 0
static const char g_module_name[] = "sf_periodic_adc";
#endif

/***********************************************************************************************************************
 * Implementation of Interface
 **********************************************************************************************************************/
/*LDRA_INSPECTED 27 D This structure must be accessible in user code. It cannot be static. */
const sf_adc_periodic_api_t g_sf_adc_periodic_on_sf_adc_periodic =
{
        .open          = SF_ADC_PERIODIC_Open,
        .start         = SF_ADC_PERIODIC_Start,
        .stop          = SF_ADC_PERIODIC_Stop,
        .close         = SF_ADC_PERIODIC_Close,
        .versionGet    = SF_ADC_PERIODIC_VersionGet
};

/*******************************************************************************************************************//**
 * @addtogroup SF_ADC_PERIODIC
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Configures periodic ADC framework and optionally starts the timer.
 *
 * The SF_ADC_PERIODIC_Open function acquires a mutex for the ADC Unit used, then calls the driver .open function in
 * the p_api parameter. The mutex is released following the driver layer open function.
 *
 * @retval SSP_SUCCESS               Initialization was successful.
 * @retval SSP_ERR_ASSERTION         One of the following parameters may be NULL: p_api_ctrl or p_cfg.  See HAL driver
 *                                   for other possible causes.
 * @retval SSP_ERR_INVALID_ARGUMENT  An invalid argument is used
 * @retval SSP_ERR_INTERNAL          An internal ThreadX error has occurred. This is typically a failure to create/use
 *                                   a mutex or to create an internal thread.
 * @return                           See @ref Common_Error_Codes or HAL driver for other possible return codes or causes.
 *
 **********************************************************************************************************************/
ssp_err_t SF_ADC_PERIODIC_Open (sf_adc_periodic_ctrl_t * const p_api_ctrl, sf_adc_periodic_cfg_t const * const p_cfg)
{
    sf_adc_periodic_instance_ctrl_t * p_ctrl = (sf_adc_periodic_instance_ctrl_t *) p_api_ctrl;

    ssp_err_t err = SSP_SUCCESS;
    UINT tx_err;

#if SF_ADC_PERIODIC_CFG_PARAM_CHECKING_ENABLE
    err = sf_adc_periodic_parameter_check(p_ctrl, p_cfg);
    SF_ADC_PERIODIC_ERROR_RETURN(SSP_SUCCESS == err, err);
#endif

    /** Save driver structure pointer for use in other framework layer functions */
    p_ctrl->p_lower_lvl_adc = p_cfg->p_lower_lvl_adc;
    p_ctrl->p_lower_lvl_timer = p_cfg->p_lower_lvl_timer;
    p_ctrl->p_lower_lvl_transfer = p_cfg->p_lower_lvl_transfer;

    /** Create a mutex */
    tx_err = tx_mutex_create(&p_ctrl->mutex, NULL, TX_INHERIT);
    SF_ADC_PERIODIC_ERROR_RETURN(TX_SUCCESS == tx_err, SSP_ERR_INTERNAL);

    /** Initialize the HAL layer*/
    err = sf_adc_periodic_initialize_hal(p_ctrl, p_cfg);

    /** If any of the HAL layer initializations failed, then delete the mutex and exit the function with the error code */
    if (SSP_SUCCESS != err)
    {
        /** Delete the mutex. */
        /*  The return codes are not checked because delete cannot fail
         *  when called with successfully created RTOS objects. */
        tx_mutex_delete(&p_ctrl->mutex);
    }

    /* Check for error */
    SF_ADC_PERIODIC_ERROR_RETURN(SSP_SUCCESS == err, err);

    /** Mark control block open so other tasks know it is valid */
    p_ctrl->open = SF_ADC_PERIODIC_OPEN;

    return SSP_SUCCESS;
} /* End of function SF_ADC_PERIODIC_Open */

/******************************************************************************************************************//**
 * @brief Gets mutex, starts the periodic ADC scan, and releases mutex.
 * @warning The driver will enable the ADC to be triggered via timer event; there will be a time delay from
 * the time this function is called to the time the hardware timer count expires and triggers the scan.
 * @retval  SSP_SUCCESS          ADC Periodic Scan started successfully.
 * @retval  SSP_ERR_ASSERTION    One or more pointers point to NULL.
 * @retval  SSP_ERR_NOT_OPEN     Driver control block not valid.  Call ::SF_ADC_PERIODIC_Open to configure.
 * @retval  SSP_ERR_INTERNAL     An internal ThreadX error has occurred. This is typically a failure to create/use
 *                               a mutex or to create an internal thread.
 * @retval  SSP_ERR_IN_USE       The module is currently busy performing another operation
 * @return                       See @ref Common_Error_Codes or HAL driver for other possible
 *                               return codes or causes. This function calls:
 *                                 * adc_api_t::scanStart
 *                                 * timer_api_t::start
 *********************************************************************************************************************/
ssp_err_t SF_ADC_PERIODIC_Start (sf_adc_periodic_ctrl_t * const p_api_ctrl)
{
    sf_adc_periodic_instance_ctrl_t * p_ctrl = (sf_adc_periodic_instance_ctrl_t *) p_api_ctrl;

#if SF_ADC_PERIODIC_CFG_PARAM_CHECKING_ENABLE
    /** Verify function pointer is valid */
    SSP_ASSERT(NULL != p_ctrl);
    SF_ADC_PERIODIC_ERROR_RETURN(SF_ADC_PERIODIC_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif

    /* Local variables used to make code more readable. */
    adc_api_t const * p_adc_api = p_ctrl->p_lower_lvl_adc->p_api;
    timer_api_t const * p_timer_api = p_ctrl->p_lower_lvl_timer->p_api;
    adc_ctrl_t * p_adc_ctrl = p_ctrl->p_lower_lvl_adc->p_ctrl;
    timer_ctrl_t * p_timer_ctrl = p_ctrl->p_lower_lvl_timer->p_ctrl;

    /** Get mutex, start timer, then release mutex */
    UINT tx_err;
    tx_err = tx_mutex_get(&p_ctrl->mutex, TX_NO_WAIT);
    SF_ADC_PERIODIC_ERROR_RETURN(TX_SUCCESS == tx_err, SSP_ERR_IN_USE);

    ssp_err_t err = SSP_SUCCESS;
    /** Enable the ADC to receive hardware triggers */
    err = p_adc_api->scanStart(p_adc_ctrl);
    /** If the scan was successfully enabled in the ADC HAL, */
    if (SSP_SUCCESS == err)
    {
        /** Start the timer to generate the ADC trigger events */
        err = p_timer_api->start(p_timer_ctrl);
    }

    /** Return the mutex. */
    /*  The return code is not checked here because tx_mutex_put cannot fail when called with
     *  a mutex owned by the current thread. The mutex is owned by the current thread because this call follows
     *  a successful call to tx_mutex_get. */
    tx_mutex_put(&p_ctrl->mutex);

    SF_ADC_PERIODIC_ERROR_RETURN(SSP_SUCCESS == err, err);
    return SSP_SUCCESS;
} /* End of function SF_ADC_PERIODIC_Start */

/******************************************************************************************************************//**
 * @brief Gets mutex, stops the periodic ADC scan, and releases mutex.
 *
 * @retval  SSP_SUCCESS          Periodic ADC scan stopped successfully.
 * @retval  SSP_ERR_ASSERTION    One or more pointers point to NULL..
 * @retval  SSP_ERR_NOT_OPEN     Driver control block not valid.  Call ::SF_ADC_PERIODIC_Open to configure.
 * @retval  SSP_ERR_INTERNAL     An internal ThreadX error has occurred. This is typically a failure to create/use
 *                               a mutex or to create an internal thread.
 * @retval  SSP_ERR_IN_USE       The module is currently busy performing another operation
 * @return                       See @ref Common_Error_Codes or HAL driver for other possible
 *                               return codes or causes. This function calls:
 *                                 * timer_api_t::stop
 *********************************************************************************************************************/
ssp_err_t SF_ADC_PERIODIC_Stop (sf_adc_periodic_ctrl_t * const p_api_ctrl)
{
    sf_adc_periodic_instance_ctrl_t * p_ctrl = (sf_adc_periodic_instance_ctrl_t *) p_api_ctrl;
    ssp_err_t err = SSP_SUCCESS;

#if SF_ADC_PERIODIC_CFG_PARAM_CHECKING_ENABLE
    /** Verify function pointer is valid */
    SSP_ASSERT(NULL != p_ctrl);
    SF_ADC_PERIODIC_ERROR_RETURN(SF_ADC_PERIODIC_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif

    /* Local variables used to make code more readable. */
    timer_api_t const * p_timer_api = p_ctrl->p_lower_lvl_timer->p_api;
    timer_ctrl_t * p_timer_ctrl = p_ctrl->p_lower_lvl_timer->p_ctrl;

    /** Get mutex, stop timer, then release mutex */
    UINT tx_err;
    tx_err = tx_mutex_get(&p_ctrl->mutex, TX_NO_WAIT);
    SF_ADC_PERIODIC_ERROR_RETURN(TX_SUCCESS == tx_err, SSP_ERR_IN_USE);

    err = p_timer_api->stop(p_timer_ctrl);

    /** Return the mutex. */
    /*  The return code is not checked here because tx_mutex_put cannot fail when called with
     *  a mutex owned by the current thread. The mutex is owned by the current thread because this call follows
     *  a successful call to tx_mutex_get. */
    tx_mutex_put(&p_ctrl->mutex);

    SF_ADC_PERIODIC_ERROR_RETURN(SSP_SUCCESS == err, err);
    return SSP_SUCCESS;
} /* End of function SF_ADC_PERIODIC_Stop */

/*******************************************************************************************************************//**
 * @brief The close function acquires the unit's mutex, closes all lower level drivers, releases and deletes the mutex.
 *
 * @retval  SSP_SUCCESS          Successful close.
 * @retval  SSP_ERR_ASSERTION    One or more pointers point to NULL.
 * @retval  SSP_ERR_NOT_OPEN     Driver control block not valid.  Call ::SF_ADC_PERIODIC_Open to configure.
 **********************************************************************************************************************/
ssp_err_t SF_ADC_PERIODIC_Close (sf_adc_periodic_ctrl_t * const p_api_ctrl)
{
    sf_adc_periodic_instance_ctrl_t * p_ctrl = (sf_adc_periodic_instance_ctrl_t *) p_api_ctrl;

#if SF_ADC_PERIODIC_CFG_PARAM_CHECKING_ENABLE
    /** Verify control block is valid */
    SSP_ASSERT(NULL != p_ctrl);
    SF_ADC_PERIODIC_ERROR_RETURN(SF_ADC_PERIODIC_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN);
#endif

    /* Error codes of APIs called during close are not checked because returning error codes without completing the
       close process leaves the module in an undefined and unrecoverable state. */
    /** Get mutex since this will access hardware registers */
    tx_mutex_get(&p_ctrl->mutex, (uint32_t)TX_NO_WAIT);

    /** Close the HAL layer modules */
    sf_adc_periodic_close_hal(p_ctrl);

    /** Delete RTOS services used */
    tx_mutex_delete(&p_ctrl->mutex);

    /** Clear information from control block so other functions know this instance is closed */
    p_ctrl->open = 0U;

    return SSP_SUCCESS;
} /* End of function SF_ADC_PERIODIC_Close */

/******************************************************************************************************************//**
 * @brief Gets version and stores it in provided pointer p_version.
 *
 * @retval SSP_SUCCESS           Version returned successfully.
 * @retval SSP_ERR_ASSERTION     Parameter p_version was null.
 *********************************************************************************************************************/
ssp_err_t SF_ADC_PERIODIC_VersionGet (ssp_version_t * const p_version)
{
#if SF_ADC_PERIODIC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_version);
#endif

    p_version->version_id = s_sf_adc_periodic_version.version_id;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup SF_ADC_PERIODIC)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/******************************************************************************************************************//**
 * @brief Close the HAL layer modules
 * @param p_ctrl Control block for this framework
 *********************************************************************************************************************/
static void sf_adc_periodic_close_hal(sf_adc_periodic_instance_ctrl_t * const p_ctrl)
{
    /** Close HAL layer drivers */
    p_ctrl->p_lower_lvl_timer->p_api->close(p_ctrl->p_lower_lvl_timer->p_ctrl);

    p_ctrl->p_lower_lvl_adc->p_api->close(p_ctrl->p_lower_lvl_adc->p_ctrl);

    /** Close the transfer driver only if lower level driver is ADC */
    if (false == p_ctrl->lower_level)
    {
        p_ctrl->p_lower_lvl_transfer->p_api->close(p_ctrl->p_lower_lvl_transfer->p_ctrl);
    }

}
/******************************************************************************************************************//**
 * @brief Initialize the HAL layer modules
 *
 * @param[in,out]  p_ctrl               Pointer to control structure.
 * @param[in]  p_cfg                    Pointer to configuration structure.
 *
 * @retval SSP_SUCCESS                  The modules were successfully initialized.
 * @retval SSP_ERR_IN_USE               One of the HAL modules is in use
 * @retval SSP_ERR_INVALID_ARGUMENT     An invalid argument is used
 * @retval SSP_ERR_INSUFFICIENT_SPACE   Not enough space in the user buffer for the number of channels specified.
 * @return                              See @ref Common_Error_Codes or HAL driver for other possible
 *                                      return codes or causes. This function calls:
 *                                          * adc_api_t::open
 *                                          * adc_api_t::scanCfg
 *                                          * adc_api_t::infoGet
 *                                          * timer_api_t::open
 *                                          * timer_api_t::infoGet
 *                                          * adc_api_t::close
 *                                          * timer_api_t::open
 *                                          * timer_api_t::close
 *                                          * elc_api_t::linkSet
 *********************************************************************************************************************/
static ssp_err_t sf_adc_periodic_initialize_hal(sf_adc_periodic_instance_ctrl_t * const p_ctrl,
        sf_adc_periodic_cfg_t const * const p_cfg)
{
    ssp_err_t err = SSP_SUCCESS;
    elc_event_t elc_event = {(elc_event_t) 0U};
    timer_info_t timer_info;
    timer_info.elc_event=  (elc_event_t)NULL;
    /* Local variables used to make code more readable. */
    adc_info_t adc_info = {0};
    adc_api_t const * p_adc_api = p_ctrl->p_lower_lvl_adc->p_api;
    timer_api_t const * p_timer_api = p_ctrl->p_lower_lvl_timer->p_api;
    adc_ctrl_t * p_adc_ctrl = p_ctrl->p_lower_lvl_adc->p_ctrl;
    timer_ctrl_t * p_timer_ctrl = p_ctrl->p_lower_lvl_timer->p_ctrl;

    /** Make a local copy of the timer configuration structure to modify it */
    timer_cfg_t hal_timer_cfg = *(p_cfg->p_lower_lvl_timer->p_cfg);

    /** Set the callback to NULL */
    hal_timer_cfg.p_callback = NULL;
    hal_timer_cfg.p_context  = NULL;

    /** Declare a local instance of the ADC HAL configuration structure*/
    adc_cfg_t hal_adc_cfg;
    /** Make a local copy of the ADC configuration structure to modify it */
    memcpy(&hal_adc_cfg, p_cfg->p_lower_lvl_adc->p_cfg, sizeof(hal_adc_cfg));

    /**Intercept the HAL callback function */
    hal_adc_cfg.p_callback = &sf_adc_periodic_callback;
    /** Set the HAL ADC drive context to the handle for the framework module to use in the ISR */
    hal_adc_cfg.p_context = p_ctrl;

    /** Initialize the ADC HAL driver */
    err = p_adc_api->open(p_adc_ctrl, &hal_adc_cfg);
    if (SSP_SUCCESS == err)
    {
        /** Initialize the ADC HAL Channel configuration */
        err = p_adc_api->scanCfg(p_adc_ctrl, p_cfg->p_lower_lvl_adc->p_channel_cfg);
        if (SSP_SUCCESS == err)
        {
            /** Initialize the Timer HAL driver */
            err = p_timer_api->open(p_timer_ctrl, &hal_timer_cfg);
            if (SSP_SUCCESS == err)
            {
                p_ctrl->p_data_buffer = p_cfg->p_data_buffer;
                p_ctrl->data_buffer_length = p_cfg->data_buffer_length;
                p_ctrl->data_buffer_index = 0U;
                p_ctrl->sample_count = p_cfg->sample_count;
                p_ctrl->current_sample_count = p_cfg->sample_count;
                p_ctrl->lower_level = p_cfg->lower_level;
                p_ctrl->p_callback =  p_cfg->p_callback;
                p_ctrl->p_context =  p_cfg->p_context;

                err = p_timer_api->infoGet(p_timer_ctrl, &timer_info);
                if (SSP_SUCCESS != err)
                {
                	p_timer_api->close(p_timer_ctrl);
                	p_adc_api->close(p_adc_ctrl);
                	return err;
                }

                elc_event = timer_info.elc_event;
                /** Get the DTC and ELC information from the lower level ADC driver*/
                err = p_cfg->p_lower_lvl_adc->p_api->infoGet(p_adc_ctrl, &adc_info);
                if (SSP_SUCCESS == err)
                {
                    /** Store the source address into the control so that it can be used to reset the transfer method
                    * after each block transfer*/
                    p_ctrl->p_src_transfer = (void const *)adc_info.p_address;
                    p_ctrl->size_multiplier = (uint32_t) adc_info.transfer_size;
                    p_ctrl->length = (uint16_t) adc_info.length;

                    if(false == p_ctrl->lower_level)
                    {
                        /** Configure the DTC to transfer the ADC scan data for all the channels scanned */
                        err = sf_adc_periodic_configure_dtc(p_ctrl, p_cfg, adc_info);
                    }
                }
                /** If the DTC was not successfully configured, then close the the timer
                 * and the ADC instance. Return value is not checked since the error for DTC open() is what
                 * will be passed back to user  */
                if (SSP_SUCCESS != err)
                {
                    p_timer_api->close(p_timer_ctrl);
                    p_adc_api->close(p_adc_ctrl);
                }
                else
                {
                    /** Configure the ELC to link the Timer Event to trigger the ADC Scan */
                    err = g_elc_on_elc.linkSet(adc_info.elc_peripheral, elc_event);
                }
            }
            else
            {
                /** Close the ADC HAL driver if the timer open() failed so that subsequent calls to open will not fail
                 * Return value is not checked since the error for timer open() is what will be passed back to user */
                p_adc_api->close(p_adc_ctrl);
            }
        }
        else
        {
            /** Close the ADC HAL driver if the scanCfg() failed so that subsequent calls to open will not fail
             * Return value is not checked since the error for scanCfg() is what will be passed back to user */
            p_adc_api->close(p_adc_ctrl);
        }
    }

    return err;
}
#if SF_ADC_PERIODIC_CFG_PARAM_CHECKING_ENABLE
/******************************************************************************************************************//**
 * @brief Checks the initialization function parameters for validity
 *
 * @param[in]  p_ctrl                Pointer to control structure.
 * @param[in]  p_cfg                 Pointer to configuration structure.
 *
 * @retval SSP_SUCCESS               The parameters are valid and the module is not in use.
 * @retval SSP_ERR_ASSERTION         One of the following parameters may be NULL: p_ctrl or p_cfg.
 * @retval SSP_ERR_IN_USE            The module is in use
 * @retval SSP_ERR_INVALID_ARGUMENT  An invalid argument is used
 * @return                           See @ref Common_Error_Codes or HAL driver for other possible return codes or causes.
 *********************************************************************************************************************/
static ssp_err_t sf_adc_periodic_parameter_check(sf_adc_periodic_instance_ctrl_t * const p_ctrl,
                                                 sf_adc_periodic_cfg_t     const * const p_cfg)
{
    ssp_err_t err = SSP_SUCCESS;

    SSP_ASSERT(NULL != p_ctrl);
    SF_ADC_PERIODIC_ERROR_RETURN(SF_ADC_PERIODIC_OPEN != p_ctrl->open, SSP_ERR_IN_USE);
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_cfg->p_callback);
    SSP_ASSERT(NULL != p_cfg->p_lower_lvl_adc);
    SSP_ASSERT(NULL != p_cfg->p_lower_lvl_timer);
    if (false == p_cfg->lower_level)
    {
        SSP_ASSERT(NULL != p_cfg->p_lower_lvl_transfer);
    }
    SF_ADC_PERIODIC_ERROR_RETURN(0U != p_cfg->sample_count, SSP_ERR_INVALID_ARGUMENT);

    return err;
}
#endif
/******************************************************************************************************************//**
 * @brief Configures the DTC
 *
 * @param[in,out]  p_ctrl                 Pointer to control structure.
 * @param[in]  p_cfg                      Pointer to configuration structure.
 * @param[in]  adc_info                   Structure holding the ADC information for transfer interface
 *
 * @retval  SSP_SUCCESS                   ADC Periodic Scan started successfully.
 * @retval  SSP_ERR_INSUFFICIENT_SPACE    Not enough space in the user buffer for the number of channels specified.
 * @return                                See @ref Common_Error_Codes or HAL driver for other possible
 *                                        return codes or causes. This function calls:
 *                                          * transfer_api_t::open
 *                                          * transfer_api_t::enable
 *
 *********************************************************************************************************************/
static ssp_err_t sf_adc_periodic_configure_dtc(sf_adc_periodic_instance_ctrl_t * const p_ctrl,
                                        sf_adc_periodic_cfg_t const * const p_cfg, adc_info_t adc_info)
{
    ssp_err_t err = SSP_SUCCESS;
    /* Local variables used to make code more readable. */
    transfer_ctrl_t * p_transfer_ctrl = p_ctrl->p_lower_lvl_transfer->p_ctrl;

    /** Make a local copy of the transfer configuration structure to modify it */
    transfer_cfg_t hal_transfer_cfg = *(p_cfg->p_lower_lvl_transfer->p_cfg);

    /**Configure the DTC trigger event as the ADC scan completion interrupt */
    hal_transfer_cfg.activation_source = adc_info.elc_event;

    /** Store the retrieved DTC transfer length to the configuration structure */
    uint16_t length = (uint16_t) adc_info.length;
    hal_transfer_cfg.p_info->length = length;
    hal_transfer_cfg.p_info->p_src = (void const *)adc_info.p_address;
    hal_transfer_cfg.p_info->size = adc_info.transfer_size;
    hal_transfer_cfg.p_info->p_dest = p_cfg->p_data_buffer;
    hal_transfer_cfg.p_info->num_blocks = (uint16_t)p_cfg->sample_count;
    /** Total DTC transfer length is the length of each transfer x the total samples requested */
    p_ctrl->dtc_transfer_length = (adc_info.length * p_ctrl->sample_count);
    /** Since a double buffering scheme is used, total number of requested samples/channel
     * must be at least twice the size of the user provided buffer. If not, then return an error */
    if ((p_ctrl->dtc_transfer_length * 2 * p_ctrl->size_multiplier) > p_ctrl->data_buffer_length)
    {
        err = SSP_ERR_INSUFFICIENT_SPACE;
    }
    else
    {    /** Initialize the DTC */
        err = p_cfg->p_lower_lvl_transfer->p_api->open(p_transfer_ctrl, &hal_transfer_cfg);

        if (SSP_SUCCESS == err)
        {
            err = p_cfg->p_lower_lvl_transfer->p_api->enable(p_transfer_ctrl);
        }
    }

    return err;

}

/*******************************************************************************************************************//**
 * @brief  Callback function intercepted from HAL layer.
 * @param  p_args Callback data can be used to identify what triggered the interrupt.
 **********************************************************************************************************************/
static void sf_adc_periodic_callback (adc_callback_args_t * p_args)
{
    sf_adc_periodic_callback_args_t args;
    sf_adc_periodic_instance_ctrl_t * p_ctrl = NULL;
    transfer_api_t const * p_transfer_api;
    uint16_t const volatile * p_src;

#if SF_ADC_PERIODIC_CFG_PARAM_CHECKING_ENABLE
    if((NULL == p_args) || (NULL == p_args->p_context))
    {
        return;
    }
#endif

    if (ADC_EVENT_SCAN_COMPLETE != p_args->event)
    {
        return;
    }

    /** Recover context from ISR */
    p_ctrl = (sf_adc_periodic_instance_ctrl_t *) (p_args->p_context);

#if SF_ADC_PERIODIC_CFG_PARAM_CHECKING_ENABLE
    /** Verify control block is valid */
    if (SF_ADC_PERIODIC_OPEN != p_ctrl->open)
    {
        return;
    }
#endif

    if (false == p_ctrl->lower_level)
    {
        /** Save the current index, data buffer and number of new samples in the data buffer to the
         *  callback argument before it is updated for the next DTC transfer */
        /* Local variables used to make code more readable. */
        p_transfer_api = p_ctrl->p_lower_lvl_transfer->p_api;
        transfer_ctrl_t * p_transfer_ctrl = p_ctrl->p_lower_lvl_transfer->p_ctrl;

        /** Save the current index to the callback argument before it is updated for the next DTC transfer */
        args.buffer_index = p_ctrl->data_buffer_index;
        args.p_data_buffer = p_ctrl->p_data_buffer;
        args.num_new_samples = p_ctrl->dtc_transfer_length;

        /** Increment the buffer index by the size of the DTC transfer so that it is pointing to the next
        * spot in the buffer for the DTC to move data to */
        p_ctrl->data_buffer_index += p_ctrl->dtc_transfer_length;

        /** If there is no more space in the buffer for another DTC transfer */
        if (((p_ctrl->data_buffer_index + p_ctrl->dtc_transfer_length) * p_ctrl->size_multiplier) > (p_ctrl->data_buffer_length ))
        {
            /** Reset the index to the start of the buffer*/
            p_ctrl->data_buffer_index = 0U;
        }

        /** Reset the DTC with the new count and destination address */
        p_transfer_api->reset(p_transfer_ctrl,  p_ctrl->p_src_transfer,
             &(p_ctrl->p_data_buffer[(p_ctrl->data_buffer_index * p_ctrl->size_multiplier)]), (uint16_t)p_ctrl->sample_count);

        /** Populate the callback argument fields */
        args.event = SF_ADC_PERIODIC_EVENT_NEW_DATA;
        args.p_context = p_ctrl->p_context;
        /** Call the user callback */
        p_ctrl->p_callback(&args);
    }
    else
    {
        uint32_t temp_buffer_index = p_ctrl->data_buffer_index * p_ctrl->size_multiplier;
        p_src = p_ctrl->p_src_transfer;
        for(uint32_t i = 0U; i< (p_ctrl->length * p_ctrl->size_multiplier); i++)
        {
            p_ctrl->p_data_buffer[temp_buffer_index++] = (p_src[i]);
        }

        p_ctrl->data_buffer_index += p_ctrl->length;
        p_ctrl->current_sample_count--;
        if (0U == p_ctrl->current_sample_count)
        {
            args.buffer_index = p_ctrl->data_buffer_index;
            args.p_data_buffer = p_ctrl->p_data_buffer;
            args.num_new_samples = (p_ctrl->length * p_ctrl->sample_count);

            /** If there is no more space in the buffer for another transfer */
            if ((p_ctrl->data_buffer_index + (p_ctrl->length * p_ctrl->sample_count)) > (p_ctrl->data_buffer_length ))
            {
                /** Reset the index to the start of the buffer*/
                p_ctrl->data_buffer_index = 0U;
            }

            /** Populate the callback argument fields */
            args.event = SF_ADC_PERIODIC_EVENT_NEW_DATA;
            args.p_context = p_ctrl->p_context;
            /** Call the user callback */
            p_ctrl->p_callback(&args);
            p_ctrl->current_sample_count = p_ctrl->sample_count;
        }
    }
} /* End of function sf_adc_periodic_callback */


