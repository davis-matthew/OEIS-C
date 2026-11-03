/*
 * A350677  a(n) is the sum of the numbers k < n such that a(k) AND n = 0
 *          (where AND denotes the bitwise AND operator).
 * https://oeis.org/A350677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum_{k < n, a(k) AND n = 0} k, a(0) = a(1) = 0
 *            Recurrence: a(n) depends on all earlier a(k). Implementation
 *            uses memoized dynamic allocation; the function works correctly
 *            regardless of the order of calls
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9893 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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
    /* static cache for computed values */
    static seq_t *cache = NULL;     /* array of a(k) values */
    static size_t cache_len = 0;    /* number of filled entries */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* ensure cache can hold indices up to n */
    if (n >= cache_len)
    {
        size_t new_cap = (size_t)n + 1;          /* need indices 0..n */
        ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

        /* compute missing values from cache_len up to n */
        for (size_t i = cache_len; i <= n; ++i)
        {
            if (i < 2)          /* base cases */
            {
                cache[i] = 0;
                continue;
            }

            seq_t sum = 0;
            seq_t mask = (seq_t)i;   /* n cast to seq_t for bitwise AND */

            for (idx_t k = 0; k < i; ++k)
            {
                if ((cache[k] & mask) == 0)
                    sum += k;
            }
            cache[i] = sum;
        }
        cache_len = new_cap;
    }

    return cache[n];
}
