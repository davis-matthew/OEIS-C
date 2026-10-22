/*
 * A390581  Antidiagonal sums of the array A389800.
 * https://oeis.org/A390581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 10*a(n-3) + 4*a(n-5) - a(n-6) for n > 5
 *            Initial values: a(0)=0, a(1)=3, a(2)=5, a(3)=22, a(4)=60,
 *            a(5)=190
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"   /* uses ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elt_size) */

static seq_t *cache = NULL;     /* dynamic cache of computed terms */
static size_t cache_len = 0;    /* number of elements currently stored */
static size_t cache_cap = 0;    /* allocated capacity */

/* Initialise cache up to index n (inclusive) and fill base terms. */
static void init_cache(idx_t n)
{
    if (cache_len > (size_t)n)
        return;                /* already big enough */

    size_t need = (size_t)n + 1;
    ensure_capacity(need, &cache_cap, (void **)&cache, sizeof(seq_t));

    for (size_t i = cache_len; i < need; ++i) {
        seq_t val = 0;
        switch (i) {
            case 0: val = 0; break;
            case 1: val = 3; break;
            case 2: val = 5; break;
            case 3: val = 22; break;
            case 4: val = 60; break;
            case 5: val = 190; break;
            default: val = 0;  /* placeholder; will be overwritten by recurrence */
        }
        cache[i] = val;
    }
    cache_len = need;

    /* Now compute remaining terms up to n if needed */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t v = 4 * cache[i - 1]
                - 10 * cache[i - 3]
                + 4 * cache[i - 5]
                - cache[i - 6];
        cache[i] = v;
    }
}

seq_t a(idx_t n)
{
    init_cache(n);
    return cache[n];
}
