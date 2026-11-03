/*
 * A386912  The base-2 logarithm of this sequence equals the Q statistic value
 *          of the greedy-from-the-bottom tree with n leaves, which is the
 *          minimal Q statistic value for n.
 * https://oeis.org/A386912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 1598 (2026-11-03)
 * Helpers:   ilog2, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: a(1) = 1
 * a(n) = n * a(f) * a(n - f)   for n > 1
 * where  
 *   b = 2^floor(log2(n-1))
 *   f = min(b, n - b/2)
 *
 * This implementation uses memoisation so that each distinct value of
 * a(n) is computed only once, even for non‑sequential calls.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ilog2.h"
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

static seq_t *cache = NULL;          /* memoisation table */
static size_t cache_cap = 0;         /* current capacity */

/* initialise or enlarge the cache to hold indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if (n < 1) return; /* n == 0 is never requested */

    /* initial allocation or growth */
    if (cache == NULL) {
        ensure_capacity((size_t)(n + 1), &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        cache[1] = (seq_t)1;          /* a(1) = 1 */
    } else if (cache_cap <= (size_t)n) {
        size_t old_cap = cache_cap;
        ensure_capacity((size_t)(n + 1), &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        for (size_t i = old_cap; i < cache_cap; ++i) cache[i] = 0;
    }
}

/* recursively compute a(n) with memoisation */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    ensure_cache_size(n);
    if (cache[n] != 0) return cache[n];

    idx_t b = ((idx_t)1 << ilog2(n - 1));
    idx_t half_b = b >> 1;          /* b/2 */
    idx_t temp = n - half_b;
    idx_t f = (b <= temp) ? b : temp;   /* min(b, n - b/2) */

    seq_t left  = a(f);
    seq_t right = a(n - f);
    seq_t res = (seq_t)n * left;
    res *= right;

    cache[n] = res;
    return res;
}
