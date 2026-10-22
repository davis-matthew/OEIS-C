/*
 * A381347  Expansion of e.g.f. 1/( 1 - sin(sqrt(2)*x) / sqrt(2) ).
 * https://oeis.org/A381347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n>0, a(n) = Sum_{k=0..floor((n-1)/2)} (-2)^k *
 *            binomial(n,2k+1) * a(n-2k-1)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* number of terms stored */

    /* initialise cache with a(0) */
    if (!cache) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* out of memory */
        cache[0] = 1;
        cached = 1;
    }

    /* extend cache up to n if necessary */
    if ((idx_t)n >= (idx_t)cached) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { free(cache); return 0; }
        cache = tmp;

        for (idx_t i = (idx_t)cached; i <= n; ++i) {
            if (i == 0) { cache[0] = 1; continue; }
            seq_t sum = 0;
            idx_t maxk = (i - 1) / 2;
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t j = i - 2 * k - 1;
                seq_t term = binomial(i, 2 * k + 1);
                term = term * cache[j];
                seq_t pow2 = pow_uint(2, k);
                term = term * pow2;
                if (k & 1) term = -term;
                sum = sum + term;
            }
            cache[i] = sum;
        }
        cached = new_cap;
    }

    return cache[n];
}
