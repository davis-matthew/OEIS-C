/*
 * A366871  a(0) = 0; a(n) = n - a(floor(a(n-1)/5)).
 * https://oeis.org/A366871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = n - a(floor(a(n-1)/5)) for n > 0
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t size = 0;      /* number of elements filled in cache */
static idx_t cap  = 0;      /* allocated capacity of cache */

static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cap)
        return;
    /* grow capacity exponentially */
    idx_t new_cap = cap ? cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* memory allocation failure */
    cache = new_cache;
    if (new_cap > cap) {
        /* initialise the newly allocated region to zero to avoid UB */
        for (idx_t i = cap; i < new_cap; ++i)
            cache[i] = 0;
    }
    cap = new_cap;
}

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0) {
        /* ensure cache[0] exists */
        if (size == 0) {
            ensure_cache_capacity(1);
            cache[0] = 0;
            size = 1;
        }
        return cache[0];
    }

    /* Compute up to n if needed */
    if (n >= size) {
        ensure_cache_capacity(n + 1);
        /* start computing from current size up to n */
        for (idx_t i = size; i <= n; ++i) {
            /* idx = floor(a(i-1)/5) */
            seq_t val_prev = cache[i - 1];
            seq_t div5 = val_prev / 5;          /* seq_t division; truncates toward 0 */
            idx_t idx = (idx_t)div5;            /* index to access cache */
            /* a(i) = i - a(idx) */
            cache[i] = (seq_t)i - cache[idx];
        }
        size = n + 1;
    }
    return cache[n];
}
