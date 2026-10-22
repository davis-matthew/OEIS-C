/*
 * A360051  a(n) = Sum_{k=0..floor(n/5)} (-1)^k * binomial(n+4,5*k+4) *
 *          Catalan(k).
 * https://oeis.org/A360051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = binomial(n+4,4) - Sum_{k=0..n-5} a(k) * a(n-k-5)
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 29, i64 n <= 58, i128 n <= 115,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 1, a(1) = 5, a(2) = 15, a(3) = 35, a(4) = 70, …  */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* extend the cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                /* memory allocation failed */
        cache = tmp;

        for (idx_t i = cache_size; i < new_size; ++i) {
            /* base binomial term */
            seq_t val = binomial(i + 4, 4);

            /* subtract the convolution term if i ≥ 5 */
            if (i >= 5) {
                seq_t sum = 0;
                for (idx_t k = 0; k <= i - 5; ++k) {
                    sum += cache[k] * cache[i - 5 - k];
                }
                val -= sum;
            }

            cache[i] = val;
        }

        cache_size = new_size;
    }

    return cache[n];
}
