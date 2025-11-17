#ifndef RING_CHAR_BUFFER_H
#define RING_CHAR_BUFFER_H

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
    uint8_t          *buffer;       /* con trỏ tới vùng đệm đã cấp phát */
} ring_char_buffer_t;

/* API */
uint32_t ring_char_buffer_init   (ring_char_buffer_t *me,
                                  uint8_t *buffer,
                                  uint32_t max_items);
uint32_t ring_char_buffer_reset  (ring_char_buffer_t *me);

uint32_t ring_char_buffer_put    (ring_char_buffer_t *me, uint8_t item);
uint32_t ring_char_buffer_get    (ring_char_buffer_t *me, uint8_t *out_item);
uint32_t ring_char_buffer_peek   (ring_char_buffer_t *me, uint8_t *out_item);

/* Ghi đè phần tử cũ khi đầy */
uint32_t ring_char_buffer_overwrite(ring_char_buffer_t *me, uint8_t item);

/* Truy vấn (trả kết quả qua out param) */
uint32_t ring_char_buffer_is_empty(ring_char_buffer_t *me, bool *out_empty);
uint32_t ring_char_buffer_is_full (ring_char_buffer_t *me, bool *out_full);
uint32_t ring_char_buffer_get_count(ring_char_buffer_t *me, uint32_t *out_count);
uint32_t ring_char_buffer_get_free (ring_char_buffer_t *me, uint32_t *out_free);

#endif /* RING_CHAR_BUFFER_H */
