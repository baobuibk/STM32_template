#include "ring_char_buffer.h"
#include <string.h>

static inline uint32_t next_idx(uint32_t cur, uint32_t max_items)
{
    return (cur + 1u) % max_items;
}

uint32_t ring_char_buffer_init(ring_char_buffer_t *me,
                               uint8_t *buffer,
                               uint32_t max_items)
{
    if (!me || !buffer || max_items < 2u) return ERROR_INVALID_PARAM;
    me->head = 0u;
    me->tail = 0u;
    me->max_items = max_items;
    me->buffer = buffer;
    return ERROR_OK;
}

uint32_t ring_char_buffer_reset(ring_char_buffer_t *me)
{
    if (!me) return ERROR_INVALID_PARAM;
    os_crit_enter();
    me->head = me->tail = 0u;
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_char_buffer_put(ring_char_buffer_t *me, uint8_t item)
{
    if (!me || !me->buffer || me->max_items < 2u) return ERROR_INVALID_PARAM;

    os_crit_enter();
    uint32_t h = me->head;
    uint32_t t = me->tail;
    if (next_idx(h, me->max_items) == t)
    {
        os_crit_exit();
        return ERROR_BUFFER_FULL;
    }
    me->buffer[h] = item;
    me->head = next_idx(h, me->max_items);
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_char_buffer_overwrite(ring_char_buffer_t *me, uint8_t item)
{
    if (!me || !me->buffer || me->max_items < 2u) return ERROR_INVALID_PARAM;

    os_crit_enter();
    uint32_t h = me->head;
    uint32_t t = me->tail;

    /* nếu đầy, đẩy tail */
    if (next_idx(h, me->max_items) == t)
    {
        me->tail = next_idx(t, me->max_items);
    }
    me->buffer[h] = item;
    me->head = next_idx(h, me->max_items);
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_char_buffer_get(ring_char_buffer_t *me, uint8_t *out_item)
{
    if (!me || !out_item) return ERROR_INVALID_PARAM;

    os_crit_enter();
    if (me->head == me->tail)
    {
        os_crit_exit();
        return ERROR_BUFFER_EMPTY;
    }
    *out_item = me->buffer[me->tail];
    me->tail = next_idx(me->tail, me->max_items);
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_char_buffer_peek(ring_char_buffer_t *me, uint8_t *out_item)
{
    if (!me || !out_item) return ERROR_INVALID_PARAM;

    os_crit_enter();
    if (me->head == me->tail)
    {
        os_crit_exit();
        return ERROR_BUFFER_EMPTY;
    }
    *out_item = me->buffer[me->tail];
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_char_buffer_is_empty(ring_char_buffer_t *me, bool *out_empty)
{
    if (!me || !out_empty) return ERROR_INVALID_PARAM;
    os_crit_enter();
    *out_empty = (me->head == me->tail);
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_char_buffer_is_full(ring_char_buffer_t *me, bool *out_full)
{
    if (!me || !out_full) return ERROR_INVALID_PARAM;
    os_crit_enter();
    *out_full = (next_idx(me->head, me->max_items) == me->tail);
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_char_buffer_get_count(ring_char_buffer_t *me, uint32_t *out_count)
{
    if (!me || !out_count) return ERROR_INVALID_PARAM;
    os_crit_enter();
    uint32_t h = me->head, t = me->tail, m = me->max_items;
    *out_count = (h >= t) ? (h - t) : (m - (t - h));
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_char_buffer_get_free(ring_char_buffer_t *me, uint32_t *out_free)
{
    if (!me || !out_free) return ERROR_INVALID_PARAM;
    os_crit_enter();
    uint32_t h = me->head, t = me->tail, m = me->max_items;
    /* one slot is reserved to distinguish full vs empty */
    uint32_t used = (h >= t) ? (h - t) : (m - (t - h));
    *out_free = (m - 1u) - used;
    os_crit_exit();
    return ERROR_OK;
}
