#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "osal.h"

/* Mã lỗi mặc định (bỏ nếu bạn đã có nơi khác) */
#ifndef ERROR_OK
#define ERROR_OK                    (0u)
#endif
#ifndef ERROR_INVALID_PARAM
#define ERROR_INVALID_PARAM         (1u)
#endif
#ifndef ERROR_OUT_OF_MEMORY
#define ERROR_OUT_OF_MEMORY         (2u)
#endif
#ifndef ERROR_BUFFER_EMPTY
#define ERROR_BUFFER_EMPTY          (3u)
#endif
#ifndef ERROR_BUFFER_FULL
#define ERROR_BUFFER_FULL           (4u)
#endif

typedef struct
{
    volatile uint32_t head;
    volatile uint32_t tail;
    uint32_t          max_items;
    uint32_t          item_size;   /* kích thước 1 phần tử */
    uint8_t          *buffer;      /* con trỏ tới vùng đệm thô */
    uint32_t          buffer_size; /* tổng bytes của buffer */
} ring_buffer_t;

/* API */
uint32_t ring_buffer_init  (ring_buffer_t *me,
                            void *buffer,
                            uint32_t buffer_size,
                            uint32_t max_items,
                            uint32_t item_size);
uint32_t ring_buffer_reset (ring_buffer_t *me);

uint32_t ring_buffer_put   (ring_buffer_t *me, const void *item);
uint32_t ring_buffer_get   (ring_buffer_t *me, void *out_item);
uint32_t ring_buffer_peek  (ring_buffer_t *me, void *out_item);

uint32_t ring_buffer_is_empty(ring_buffer_t *me, bool *out_empty);
uint32_t ring_buffer_is_full (ring_buffer_t *me, bool *out_full);
uint32_t ring_buffer_get_count(ring_buffer_t *me, uint32_t *out_count);
uint32_t ring_buffer_get_free (ring_buffer_t *me, uint32_t *out_free);

#endif /* RING_BUFFER_H */
