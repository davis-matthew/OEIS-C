/*
 * A287475  a(0) = 0; a(1) = 1; a(2*n) = a(n), a(2*n+1) = a(n-a(n)) +
 *          a(n-a(n+1)).
 * https://oeis.org/A287475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(1) = 1; a(2*n) = a(n); a(2*n+1) = a(n - a(n)) + a(n
 *            - a(n+1));
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 101 terms through n = 100 (2026-11-03)
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
    /* static cache: a[0..capacity-1] holds computed values */
    static seq_t *cache   = NULL;
    static idx_t  capacity = 0;      /* number of slots currently allocated */
    static idx_t  last_comp = 1;     /* highest index for which cache is filled */

    /* initialise cache with first two terms */
    if (cache == NULL) {
        ensure_capacity(2, &capacity, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        last_comp = 1;
    }

    /* enlarge cache if necessary (need n+1 elements) */
    if ((idx_t)capacity <= n) {
        ensure_capacity(n + 1, &capacity, OEISC_VPP(&cache), sizeof(seq_t));
    }

    /* compute values from last_comp+1 up to n (inclusive) */
    for (idx_t m = last_comp + 1; m <= n; ++m) {
        seq_t val;
        if ((m & 1U) == 0) {                 /* even: a(2*k) = a(k) */
            idx_t k = m >> 1U;
            val = cache[k];
        } else {                              /* odd:  a(2*k+1) */
            idx_t k  = (m - 1U) >> 1U;        /* k = (m-1)/2 */
            idx_t i1 = k - (idx_t)cache[k];    /* k - a(k)   */
            idx_t i2 = k - (idx_t)cache[k + 1U];/* k - a(k+1) */
            val = cache[i1] + cache[i2];
        }
        cache[m] = val;
    }
    if (n > last_comp) last_comp = n;

    return cache[n];
}
