/*
 * A349016  G.f. A(x) satisfies: A(x) = 1 + x * A(-x) / (1 - x) + x * A(x)^2.
 * https://oeis.org/A349016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} a(k) * ((-1)^k + a(n-k-1))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A349016 with memoised calculation up to any n. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation across calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with first term */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1; /* a(0) = 1 */
        if (n == 0) return cache[0];
    }

    /* If requested n is beyond current cache, extend it */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        /* Allocate new buffer */
        seq_t *new_buf = (seq_t*)malloc((size_t)new_len * sizeof(seq_t));
        if (!new_buf) abort();
        /* Copy existing values */
        memcpy(new_buf, cache, old_len * sizeof(seq_t));
        free(cache);
        cache = new_buf;
        cache_len = new_len;

        /* Compute new terms from old_len to n */
        for (idx_t i = old_len; i < new_len; ++i) {
            seq_t sum_conv = 0;
            seq_t sum_even = 0;
            seq_t sum_odd  = 0;

            /* k runs from 0 to i-1 */
            for (idx_t k = 0; k < i; ++k) {
                seq_t ak = cache[k];
                seq_t prod = ak * cache[i - 1 - k];
                sum_conv += prod;

                if (k & 1U) {
                    sum_odd += ak;
                } else {
                    sum_even += ak;
                }
            }
            /* a(i) = sum_conv + sum_even - sum_odd  (always non‑negative) */
            cache[i] = sum_conv + sum_even - sum_odd;
        }
    }

    return cache[n];
}
