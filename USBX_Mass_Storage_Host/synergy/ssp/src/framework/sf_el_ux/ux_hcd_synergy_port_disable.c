/*LDRA_INSPECTED 119 S */
/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

/***********************************************************************************************************************
 * Copyright [2017-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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

/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   SYNERGY Controller Driver                                           */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE

#include "ux_api.h"
#include "ux_hcd_synergy.h"

/*******************************************************************************************************************//**
 * @addtogroup sf_el_ux
 * @{
 **********************************************************************************************************************/

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    ux_hcd_synergy_port_disable                         PORTABLE C      */
/*                                                           5.7 SP7      */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron, Microsoft Corporation                                */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function will disable a specific port attached to the root    */ 
/*     HUB.                                                               */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_synergy                           Pointer to Synergy controller */
/*    port_index                            Port index                    */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    ux_hcd_synergy_register_read          Read Synergy register         */
/*    ux_hcd_synergy_register_write         Write Synergy register        */
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    Synergy Controller Driver                                           */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  10-10-2012     TCRG                     Initial Version 5.6           */ 
/*  06-01-2014     TCRG                     Modified comment(s),          */ 
/*                                            resulting in version 5.7    */ 
/*  12-15-2016     TCRG                     Modified comment(s),          */ 
/*                                            added new error trap        */
/*                                            function support,           */
/*                                            resulting in version 5.7 SP7*/ 
/*                                                                        */ 
/**************************************************************************/ 

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  This function will disable a specific port attached to the root HUB.
 *
 * @param[in]  hcd_synergy : Pointer to a HCD control block
 * @param[in]  port_index  : Port Index
 *
 * @retval UX_SUCCESS              Port disabled successfully.
 * @retval UX_PORT_INDEX_UNKNOWN   Invalid port .
 **********************************************************************************************************************/
UINT  ux_hcd_synergy_port_disable(UX_HCD_SYNERGY *hcd_synergy, ULONG port_index)
{
    /* Check to see if this port is valid on this controller.  */
    if (hcd_synergy -> ux_hcd_synergy_nb_root_hubs < (UINT)port_index)
    {
        /* Error trap. */
        _ux_system_error_handler((UINT)UX_SYSTEM_LEVEL_THREAD, (UINT)UX_SYSTEM_CONTEXT_HCD, (UINT)UX_PORT_INDEX_UNKNOWN);

        /* If trace is enabled, insert this event into the trace buffer.  */
        UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_PORT_INDEX_UNKNOWN, port_index, 0, 0, UX_TRACE_ERRORS, 0, 0)

        return (UINT)UX_PORT_INDEX_UNKNOWN;
    }
    
    /* Disable the port.   */
    ux_hcd_synergy_register_clear(hcd_synergy, UX_SYNERGY_HC_DVSTCTR0, UX_SYNERGY_HC_DVSTCTR0_UACT);

    /* Return successful completion.  */
    return (UINT)UX_SUCCESS;
}
 /*******************************************************************************************************************//**
  * @} (end addtogroup sf_el_ux)
  **********************************************************************************************************************/

