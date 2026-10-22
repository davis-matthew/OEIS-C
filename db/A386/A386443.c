/*
 * A386443  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} k^2 * a(k) * a(n-1-k).
 * https://oeis.org/A386443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + Sum_{k=0}^{n-1} k^2 * a(k) * a(n-1-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with a(0) */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);                      /* fatal error if cannot allocate */
        cache[0] = (seq_t)1;
    }

    /* Grow cache if the requested index is beyond current size */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;

        /* Compute missing values from cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k) {
                /* k^2 as seq_t */
                seq_t ksq = (seq_t)k * (seq_t)k;
                /* product a(k) * a(i-1-k) */
                seq_t prod = cache[k] * cache[i - 1 - k];
                /* multiply by k^2 and add to sum */
                prod = prod * ksq;
                sum += prod;
            }
            cache[i] = cache[i - 1] + sum;
        }
        cache_size = new_size;
    }

    return cache[n];
}
