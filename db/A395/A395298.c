/*
 * A395298  a(n) = (2*n)! * [x^(2*n)] cosh(x)^5 / cos(x).
 * https://oeis.org/A395298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10 + 5*9^n + 25^n)/16 - Sum_{k=0..n-1} (-1)^(n-k) *
 *            binomial(2*n,2*k) * a(k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of elements stored */

    /* static table for binomial coefficients and its capacity */
    static seq_t *binom = NULL;
    static size_t binom_cap = 0;       /* capacity in elements */

    /* powers of 9 and 25 */
    static seq_t pow9 = 1;
    static seq_t pow25 = 1;

    /* Extend cache up to n */
    if ((size_t)n >= cache_size) {
        size_t new_cache_size = (size_t)n + 1;
        /* realloc with explicit cast to avoid C++ warning */
        cache = (seq_t*)realloc(cache,
                                new_cache_size * sizeof(seq_t));
        if (!cache) { /* in practice we could abort, but omitted for brevity */
            return (seq_t)0;
        }

        for (idx_t i = cache_size; i <= n; ++i) {
            /* update powers for current exponent */
            if (i > 0) {
                pow9   = pow9   * (seq_t)9;
                pow25  = pow25  * (seq_t)25;
            }

            if (i == 0) {
                cache[0] = 1;
                continue;
            }

            /* binomial coefficients C(2*i , j) for j=0..2*i */
            size_t needed = (size_t)2 * i + 1;
            if (needed > binom_cap) {
                free(binom);
                binom = (seq_t*)malloc(needed * sizeof(seq_t));
                if (!binom) return (seq_t)0;
                binom_cap = needed;
            }
            binom[0] = 1;
            for (idx_t j = 1; j <= 2 * i; ++j) {
                seq_t prev = binom[j - 1];
                seq_t mult = prev * ((seq_t)(2 * i - j + 1));
                binom[j] = mult / (seq_t)j;   /* exact division */
            }

            /* compute the alternating sum part */
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t bin = binom[2 * k];          /* C(2*i , 2*k) */
                seq_t term = bin * cache[k];
                if (((i - k) & 1) == 1)   /* (-1)^(i-k) == -1 */
                    sum_neg += term;
                else
                    sum_pos += term;
            }

            /* term (10 + 5*9^i + 25^i)/16 */
            seq_t tmp = (seq_t)10 + (seq_t)5 * pow9 + pow25;
            tmp /= (seq_t)16;

            /* combine */
            seq_t total = tmp + sum_neg - sum_pos;
            cache[i] = total;
        }
        cache_size = new_cache_size;
    }

    return cache[n];
}
