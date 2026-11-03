/*
 * A241682  Total number of unit squares appearing in the Thue-Morse sequence
 *          logical matrices after n stages.
 * https://oeis.org/A241682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-5) - 4*a(n-4) - 20*a(n-3) + 5*a(n-2) + 4*a(n-1)
 *            for n >= 6, with base terms: a(0)=0, a(1)=2, a(2)=0, a(3)=8,
 *            a(4)=16, a(5)=72
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 987 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
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

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* if we need more entries, grow cache */
    if ((size_t)n >= cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = (size_t)n + 1;
        ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

        /* compute newly required terms only */
        for (size_t i = old_cap; i < cache_cap; ++i) {
            if (i == 0)        cache[i] = (seq_t)0;
            else if (i == 1)   cache[i] = (seq_t)2;
            else if (i == 2)   cache[i] = (seq_t)0;
            else if (i == 3)   cache[i] = (seq_t)8;
            else if (i == 4)   cache[i] = (seq_t)16;
            else if (i == 5)   cache[i] = (seq_t)72;
            else { /* recurrence for i >= 6 */
                seq_t pos = (seq_t)16 * cache[i - 5];
                pos += (seq_t)5  * cache[i - 2];
                pos += (seq_t)4  * cache[i - 1];
                seq_t neg = (seq_t)4  * cache[i - 4];
                neg += (seq_t)20 * cache[i - 3];
                /* the recurrence guarantees pos >= neg, use unsigned subtraction */
                cache[i] = pos - neg;
            }
        }
    }
    return cache[n];
}
