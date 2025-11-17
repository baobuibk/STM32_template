#include "ring_buffer.h"
#include <string.h>
#include <limits.h>

static inline uint32_t next_idx(uint32_t cur, uint32_t max_items)
{
    return (cur + 1u) % max_items;
}

uint32_t ring_buffer_init(ring_buffer_t *me,
                          void *buffer,
                          uint32_t buffer_size,
                          uint32_t max_items,
                          uint32_t item_size)
{
    if (!me || !buffer || max_items < 2u || item_size == 0u) return ERROR_INVALID_PARAM;

    /* tránh overflow khi nhân */
    if (item_size != 0u && max_items > (UINT32_MAX / item_size)) return ERROR_OUT_OF_MEMORY;
    if ((max_items * item_size) > buffer_size) return ERROR_OUT_OF_MEMORY;

    me->head = 0u;
    me->tail = 0u;
    me->max_items = max_items;
    me->item_size = item_size;
    me->buffer    = (uint8_t*)buffer;
    me->buffer_size = buffer_size;
    return ERROR_OK;
}

uint32_t ring_buffer_reset(ring_buffer_t *me)
{
    if (!me) return ERROR_INVALID_PARAM;
    os_crit_enter();
    me->head = me->tail = 0u;
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_buffer_put(ring_buffer_t *me, const void *item)
{
    if (!me || !item || !me->buffer) return ERROR_INVALID_PARAM;

    os_crit_enter();
    uint32_t h = me->head;
    uint32_t t = me->tail;

    if (next_idx(h, me->max_items) == t)
    {
        os_crit_exit();
        return ERROR_BUFFER_FULL;
    }

    memcpy(&me->buffer[h * me->item_size], item, me->item_size);
    me->head = next_idx(h, me->max_items);
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_buffer_get(ring_buffer_t *me, void *out_item)
{
    if (!me || !out_item || !me->buffer) return ERROR_INVALID_PARAM;

    os_crit_enter();
    if (me->head == me->tail)
    {
        os_crit_exit();
        return ERROR_BUFFER_EMPTY;
    }
    memcpy(out_item, &me->buffer[me->tail * me->item_size], me->item_size);
    me->tail = next_idx(me->tail, me->max_items);
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_buffer_peek(ring_buffer_t *me, void *out_item)
{
    if (!me || !out_item || !me->buffer) return ERROR_INVALID_PARAM;

    os_crit_enter();
    if (me->head == me->tail)
    {
        os_crit_exit();
        return ERROR_BUFFER_EMPTY;
    }
    memcpy(out_item, &me->buffer[me->tail * me->item_size], me->item_size);
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_buffer_is_empty(ring_buffer_t *me, bool *out_empty)
{
    if (!me || !out_empty) return ERROR_INVALID_PARAM;
    os_crit_enter();
    *out_empty = (me->head == me->tail);
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_buffer_is_full(ring_buffer_t *me, bool *out_full)
{
    if (!me || !out_full) return ERROR_INVALID_PARAM;
    os_crit_enter();
    *out_full = (next_idx(me->head, me->max_items) == me->tail);
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_buffer_get_count(ring_buffer_t *me, uint32_t *out_count)
{
    if (!me || !out_count) return ERROR_INVALID_PARAM;
    os_crit_enter();
    uint32_t h = me->head, t = me->tail, m = me->max_items;
    *out_count = (h >= t) ? (h - t) : (m - (t - h));
    os_crit_exit();
    return ERROR_OK;
}

uint32_t ring_buffer_get_free(ring_buffer_t *me, uint32_t *out_free)
{
    if (!me || !out_free) return ERROR_INVALID_PARAM;
    os_crit_enter();
    uint32_t h = me->head, t = me->tail, m = me->max_items;
    uint32_t used = (h >= t) ? (h - t) : (m - (t - h));
    *out_free = (m - 1u) - used; /* chừa 1 slot */
    os_crit_exit();
    return ERROR_OK;
}
