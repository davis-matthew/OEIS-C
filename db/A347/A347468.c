/*
 * A347468  Numbers k such that floor(k*sqrt(3)) = floor(h*sqrt(2)) for some
 *          h.
 * https://oeis.org/A347468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Uses the intersection of the Beatty sequences for √3 and √2. k
 *            is in the sequence iff ∃h such that floor(k*√3) = floor(h*√2).
 *            The algorithm merges the two increasing floor sequences, storing
 *            the encountered k-values in a cache for subsequent queries
 * Range:     u8 n <= 2, u16 n <= 50, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static bool initialized = false;
static idx_t  next_k = 0, next_h = 0;     /* current indices in Beatty sequences */
static idx_t  valA   = 0, valB = 0;       /* current floor values */
static seq_t *cache  = NULL;             /* cached k-values */
static size_t cache_size = 0;            /* number of values stored */
static size_t cache_cap  = 0;            /* capacity of the cache buffer */

static void init_sequence(void)
{
    next_k = 1;
    next_h = 1;
    valA   = (idx_t)floor_sqrt((seq_t)3 * (seq_t)next_k * (seq_t)next_k);
    valB   = (idx_t)floor_sqrt((seq_t)2 * (seq_t)next_h * (seq_t)next_h);

    cache_cap  = 16;
    cache_size = 0;
    cache = mem_new(seq_t, cache_cap);
    /* In the unlikely event of allocation failure we let malloc fail
       naturally and crash on the subsequent use. */
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but harmless */
    if (!initialized) init_sequence();

    /* Grow cache until we have at least n entries. */
    while (cache_size < n) {
        if (valA < valB) {
            ++next_k;
            valA = (idx_t)floor_sqrt((seq_t)3 * (seq_t)next_k * (seq_t)next_k);
        } else if (valB < valA) {
            ++next_h;
            valB = (idx_t)floor_sqrt((seq_t)2 * (seq_t)next_h * (seq_t)next_h);
        } else { /* equal */
            if (cache_size == cache_cap)
                ensure_capacity(cache_size + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

            cache[cache_size++] = (seq_t)next_k;   /* store the k value */

            ++next_k;
            valA = (idx_t)floor_sqrt((seq_t)3 * (seq_t)next_k * (seq_t)next_k);
            ++next_h;
            valB = (idx_t)floor_sqrt((seq_t)2 * (seq_t)next_h * (seq_t)next_h);
        }
    }
    return cache[n - 1];
}
