/*
 * A395560  1 = Sum_{n>=0} a(n) * x^n * (1 - (3*n+1)*x)^2.
 * https://oeis.org/A395560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(3*n-2)*a(n-1) - (3*n-5)^2*a(n-2) (a(0)=1, a(1)=2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of already-computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (cache_len <= (size_t)n) {
        /* Need to grow cache */
        size_t new_len = (size_t)n + 1U;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();          /* allocation failure */
        cache = new_cache;

        /* Compute all new terms */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)2;
            } else {
                idx_t k = (idx_t)i;

                /* term1 = 2*(3*k - 2)*cache[k-1] */
                seq_t mult1 = ((seq_t)3 * k) - 2;
                seq_t term1 = (seq_t)2 * mult1 * cache[i-1];

                /* term2 = (3*k - 5)^2 * cache[k-2] */
                seq_t mult2 = ((seq_t)3 * k) - 5;
                seq_t term2 = mult2 * mult2 * cache[i-2];

                cache[i] = term1 - term2;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
