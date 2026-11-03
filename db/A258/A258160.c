/*
 * A258160  a(n) = 8*Lucas(n).
 * https://oeis.org/A258160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * Lucas(n). Recurrence: a(0) = 16, a(1) = 8, a(n) =
 *            a(n-1) + a(n-2) for n > 1
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 297 (2026-11-03)
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
    /* Static cache to store already computed values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* current allocation size (bytes) */
    static idx_t cache_len = 0;   /* number of initialized elements */

    if (cache_len <= n) {
        /* Ensure the underlying array big enough. */
        ensure_capacity((size_t)(n + 1), &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

        /* Compute missing terms. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)
                cache[0] = (seq_t)16;
            else if (i == 1)
                cache[1] = (seq_t)8;
            else
                cache[i] = cache[i - 1] + cache[i - 2];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
