/*
 * A292568  a(n) = a(n-1) + sum of base-1000 digits of a(n-1), a(0)=1.
 * https://oeis.org/A292568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   base1000_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A292568: a(0)=1; for n>0: a(n)=a(n-1)+sum_{base‑1000 digits of a(n-1)) } */
#include <stdlib.h>
#include <string.h>
#include "../utils/base1000_digit_sum.h"

/* memoised computation of a(n) – works for any order of calls */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size in elements */
    static idx_t cached_max = 0;   /* highest index currently computed */

    /* initialise on first call */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 1;              /* a(0) */
        cached_max = 0;
    }

    /* if we need more space, grow the cache */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        /* copy already computed values */
        memcpy(new_cache, cache, (cached_max + 1) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* compute forward from the next missing index up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t ds   = base1000_digit_sum(prev);
        cache[i] = prev + ds;
    }
    cached_max = n;                 /* update highest computed index */

    return cache[n];
}
