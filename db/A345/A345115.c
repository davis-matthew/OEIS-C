/*
 * A345115  Trajectory of 49 under the map x -> A345111(x).
 * https://oeis.org/A345115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 49; a(n) = a(n-1) + rotl(a(n-1)) for n ≥ 1 where rotl(x)
 *            rotates the decimal digits of x left by one position (e.g. 143
 *            -> 431, 574 -> 745)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* rotate the decimal digits of x left by one position */
static seq_t rotate_left(seq_t x)
{
    if (x < 10)                /* single‑digit numbers stay the same */
        return x;

    seq_t pow10 = 1;           /* will become 10^(d-1) */
    seq_t y = x;
    int d = 1;
    while (y >= 10)            /* count digits and compute 10^(d-1) */
    {
        y   /= 10;
        d++;
        pow10 *= 10;
    }

    seq_t first = x / pow10;   /* most significant digit */
    seq_t rem   = x % pow10;   /* the remaining part */
    return rem * 10 + first;   /* rotated value */
}

static seq_t *cache = NULL;   /* array of computed terms */
static size_t cache_len = 0;  /* current number of stored terms */
static size_t cache_cap = 0;  /* allocated capacity */

seq_t a(idx_t n)
{
    /* initialise cache for the first term if empty */
    if (cache_len == 0) {
        cache_cap = 1;
        cache     = seq_realloc(NULL, 0, cache_cap);
        if (!cache) abort();
        cache[0]   = 49;         /* a(0) = 49 */
        cache_len  = 1;
    }

    /* if requested index is already computed, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow the array if necessary, using power‑of‑two growth */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t rot  = rotate_left(prev);
        cache[i]   = prev + rot;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
