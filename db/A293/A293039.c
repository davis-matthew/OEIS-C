/*
 * A293039  E.g.f.: exp(1 + x + x^2/2! + x^3/3! - exp(x)).
 * https://oeis.org/A293039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = -Sum_{k=4..n} binomial(n-1,k-1) * a(n-k) (OEIS
 *            A293039)
 * Range:     i8 n <= 9, i16 n <= 12, i32 n <= 19, i64 n <= 29, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;

    if (n >= capacity) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
        if (cache) {
            memcpy(new_cache, cache, (size_t)capacity * sizeof(seq_t));
            free(cache);
        }
        for (idx_t i = capacity; i < new_cap; ++i) {
            if (i == 0) {
                new_cache[i] = (seq_t)1;
            } else if (i < 4) {
                new_cache[i] = (seq_t)0;
            } else {
                seq_t sum = (seq_t)0;
                seq_t binom = (seq_t)1;           /* binomial(i-1,0)               */
                idx_t limit = i - 4;
                for (idx_t j = 0; j <= limit; ++j) {
                    if (j > 0) {
                        /* binomial(i-1, j) = binom(i-1, j-1) * (i-j) / j */
                        seq_t num = (seq_t)(i - j);
                        seq_t den = (seq_t)j;
                        binom = binom * num / den;
                    }
                    sum += binom * new_cache[j];
                }
                new_cache[i] = -sum;
            }
        }
        cache = new_cache;
        capacity = new_cap;
    }
    return cache[n];
}
