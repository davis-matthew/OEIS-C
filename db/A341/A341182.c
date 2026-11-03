/*
 * A341182  Numbers that when divided by the sum of their digits leave 15 as
 *          remainder.
 * https://oeis.org/A341182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2973, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Validated: standard, pass, 64 terms through n = 4262 (2026-11-03)
 * Helpers:   digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A341182: the n‑th integer m such that m % digit_sum(m) == 15 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

static seq_t *cache = NULL;          /* cached terms */
static idx_t cache_len = 0;          /* number of cached terms */
static idx_t cache_cap = 0;          /* current capacity of cache */
static seq_t next_candidate = 1;     /* next integer to test */

seq_t a(idx_t n)
{
    /* Build the cache up to the requested index. */
    while (cache_len < n)
    {
        seq_t cand = next_candidate++;          /* current candidate integer */
        seq_t s = digit_sum(cand);              /* sum of its decimal digits */

        if (cand % s == 15)                     /* condition satisfied */
        {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap)
            {
                if (cache == NULL)
                {
                    cache_cap = 16;
                    cache = mem_new(seq_t, cache_cap);
                }
                else
                {
                    ensure_capacity((size_t)(cache_len + 1),
                                    &cache_cap,
                                    OEISC_VPP(&cache),
                                    sizeof(seq_t));
                }
            }
            cache[cache_len++] = cand;           /* store the term */
        }
    }
    return cache[n - 1];   /* 1‑based indexing */
}
