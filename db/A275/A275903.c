/*
 * A275903  Expansion of (1+4*x^2) / (1-5*x+4*x^2-4*x^3).
 * https://oeis.org/A275903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) + 4*a(n-3) for n > 2; a(0) = 1, a(1)
 *            = 5, a(2) = 25
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 997 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* static cache of computed values */
    static seq_t *cache = NULL;      /* cache array (0‑index) */
    static size_t cap   = 0;         /* allocated size of cache */
    static idx_t max_computed = 2;   /* highest n for which cache[i] is valid */

    /* initialise the cache on the first call */
    if (cache == NULL) {
        ensure_capacity(3, &cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = 1;   /* a(0) */
        cache[1] = 5;   /* a(1) */
        cache[2] = 25;  /* a(2) */
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cap) {
        /* request capacity at least n+1 */
        ensure_capacity((size_t)n + 1, &cap, OEISC_VPP(&cache), sizeof(seq_t));
    }

    /* if already computed, return immediately */
    if (n <= max_computed) {
        return cache[n];
    }

    /* compute all missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = 5 * cache[i-1] - 4 * cache[i-2] + 4 * cache[i-3];
    }
    max_computed = n;

    return cache[n];
}
