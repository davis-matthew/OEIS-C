/*
 * A276289  Expansion of x*(1 + x)/(1 - 2*x)^3.
 * https://oeis.org/A276289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 12*a(n-2) + 8*a(n-3); a(0)=0, a(1)=1, a(2)=7
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 924 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

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

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of computed terms */
    static size_t cache_cap = 0;     /* currently allocated capacity */

    /* Ensure we have storage for indices 0..n */
    ensure_capacity((size_t)n + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

    /* Compute missing terms up to and including n */
    while (cache_len <= n) {
        idx_t i = cache_len;
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)7;
        } else {
            /* recurrence: a(i) = 6*a(i-1) - 12*a(i-2) + 8*a(i-3) */
            seq_t part1 = mul_small(cache[i - 1], 6);
            seq_t part2 = mul_small(cache[i - 2], 12);
            seq_t part3 = mul_small(cache[i - 3], 8);
            seq_t sum   = part1 + part3;   /* part1 + part3 >= part2 for i>=3 */
            seq_t res   = sum - part2;
            cache[i] = res;
        }
        cache_len++;
    }
    return cache[n];
}
