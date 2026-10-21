/*
 * A293533  E.g.f.: 1/Product_{m > 0, m mod 3 > 0} exp(x^m).
 * https://oeis.org/A293533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * [x^n] exp((x + x^2)/(x^3 - 1))
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A293533, EGF: exp((x + x^2)/(x^3 - 1))              */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;

    /* expand cache if needed */
    if (n >= cache_len) {
        /* allocate new cache of size n+1 (at least one more element) */
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* memory exhaustion – abort (undefined behaviour in this context) */
            abort();
        }

        /* base case */
        if (cache_len == 0) {
            new_cache[0] = (seq_t)1;
            cache_len = 1;
        }
        /* compute missing values up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                new_cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            seq_t prod = 1;   /* product of (i-1)*(i-2)*...*(i-m+1) */
            for (idx_t m = 1; m <= i; ++m) {
                if (m % 3 != 0) {           /* m not divisible by 3 */
                    /* term = -(m * prod) * cache[i-m] */
                    seq_t term = prod * (seq_t)m;
                    term = -term;
                    term = term * new_cache[i - m];
                    sum = sum + term;
                }
                if (m < i) {
                    prod = prod * (seq_t)(i - m);
                }
            }
            new_cache[i] = sum;
        }
        cache = new_cache;
        cache_len = new_len;
    }
    return cache[n];
}
