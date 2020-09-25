/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018-2020 Amber-Sophia Schroeck
*
*The Mini Thread Library is free software; you can redistribute it and/or modify  
*it under the terms of the GNU Lesser General Public License as published by  
*the Free Software Foundation, version 3, or (at your option) any later version.

*The Mini Thread Library is distributed in the hope that it will be useful, but 
*WITHOUT ANY WARRANTY; without even the implied warranty of 
*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
*General Public License for more details.
*
*You should have received a copy of the GNU Lesser General Public
*License along with the Mini Thread  Library; if not, see
*<https://www.gnu.org/licenses/>.  
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "mn_task_utils.hpp"

bool task_utils::notify(basic_task* pTaskToNotify, uint32_t ulValue, task_utils::action action) {
    BaseType_t success;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        success = xTaskNotifyFromISR( pTaskToNotify->get_handle(),
                        ulValue,
                        (eNotifyAction)action,
                        &xHigherPriorityTaskWoken );

        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();

    } else {
        success = xTaskNotify( pTaskToNotify->get_handle(),
                        ulValue,
                        (eNotifyAction)action );
    }

  return success == pdTRUE;
}


bool task_utils::notify_unlock(basic_task* pTaskToNotify) {
    BaseType_t success = pdTRUE;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        vTaskNotifyGiveFromISR( pTaskToNotify->get_handle(), &xHigherPriorityTaskWoken );

        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();

    } else {
        success = xTaskNotifyGive( pTaskToNotify->get_handle() );
    }

  return success == pdTRUE;
}

uint32_t task_utils::notify_lock(bool bClearCountOnExit, TickType_t xTicksToWait) {
     return ulTaskNotifyTake( bClearCountOnExit ? pdTRUE : pdFALSE, 
                              xTicksToWait );
}
bool task_utils::notify_wait(uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit,
                 uint32_t *pulNotificationValue, TickType_t xTicksToWait ) {

    return xTaskNotifyWait( ulBitsToClearOnEntry, ulBitsToClearOnExit,
                            pulNotificationValue, xTicksToWait ) == pdTRUE;
}