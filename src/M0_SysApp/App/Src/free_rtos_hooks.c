/* freertos_hooks.c — Hooks chuẩn cho FreeRTOS trên Cortex-M (STM32F7)
 * Đảm bảo file này được biên dịch như C (không phải C++).
 */
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

/* =========================
 * 1) Hook: Stack overflow
 *    Bắt buộc nếu configCHECK_FOR_STACK_OVERFLOW > 0
 * ========================= */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    /* Tắt ngắt để giữ trạng thái */
    taskDISABLE_INTERRUPTS();
    (void)xTask;
    (void)pcTaskName; /* Tránh cảnh báo nếu chưa dùng */

    /* TODO: Đặt breakpoint vào đây để bắt lỗi sớm,
       hoặc chớp LED / in log qua UART */
    for (;;)
    {
        /* trap */
    }
}

/* =========================
 * 2) Hook: Malloc failed
 *    Bắt buộc nếu dùng heap_4/heap_5 và configUSE_MALLOC_FAILED_HOOK = 1
 * ========================= */
void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    for (;;)
    {
    }
}

/* =========================
 * 3) Hook: Idle (tùy chọn)
 *    Dùng khi configUSE_IDLE_HOOK = 1
 * ========================= */
void vApplicationIdleHook(void)
{
    /* Có thể đưa CPU vào sleep ở đây nếu muốn tiết kiệm năng lượng */
    __asm volatile("nop");
}

/* =========================
 * 4) Hook: Tick (tùy chọn)
 *    Dùng khi configUSE_TICK_HOOK = 1
 * ========================= */
void vApplicationTickHook(void)
{
    /* Nếu không dùng, để trống */
}

/* =========================
 * 5) Static allocation (bắt buộc nếu configSUPPORT_STATIC_ALLOCATION = 1)
 *    Cấp bộ nhớ tĩnh cho Idle Task và Timer Task
 * ========================= */
#if (configSUPPORT_STATIC_ALLOCATION == 1)

static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer   = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize   = (uint32_t)configMINIMAL_STACK_SIZE;
}

#if (configUSE_TIMERS == 1)
static StaticTask_t xTimerTaskTCB;
static StackType_t  uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer   = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize   = (uint32_t)configTIMER_TASK_STACK_DEPTH;
}
#endif /* configUSE_TIMERS */

#endif /* configSUPPORT_STATIC_ALLOCATION */

/* =========================
 * 6) Assert hook (tùy chọn)
 *    Hữu ích khi bạn dùng configASSERT
 * ========================= */
void vAssertCalled(const char *file, int line)
{
    (void)file; (void)line;
    taskDISABLE_INTERRUPTS();
    for (;;)
    {
    }
}
