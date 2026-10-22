/*
 * A367548  a(n) = Sum_{k = 0..n} binomial(-n, k) * 2^(n - k).
 * https://oeis.org/A367548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} binomial(-n, k) * 2^(n - k) * Equivalent:
 *            a(n) = Sum_{k=0..n} (-1)^k * 3^(n-k) * binomial(2n, k) *
 *            Recurrence: 9*n*a(n) +6*(6*n-7)*a(n-1) +16*(-n-4)*a(n-2)
 *            +32*(-2*n+5)*a(n-3)=0 * Initial values: a(0)=1, a(1)=1, a(2)=3
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 14, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* ensure cache has at least the first three values */
    if (cache_size == 0) {
        cache_size = 3;
        cache = (seq_t *)realloc(NULL, cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
    }

    /* return cached value if already computed */
    if ((size_t)n < cache_size) {
        return cache[n];
    }

    /* grow cache to accommodate n */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – return last known value (should not happen) */
        return cache[cache_size - 1];
    }
    cache = new_cache;

    /* compute missing terms up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i < 3) {
            continue; /* already initialized */
        }
        seq_t t1 = 6 * (seq_t)(6*i - 7) * cache[i-1];
        seq_t t2 = 16 * (-( (seq_t)i ) - 4) * cache[i-2];
        seq_t t3 = 32 * (-( (seq_t)2*i ) + 5) * cache[i-3];
        seq_t numerator = -(t1 + t2 + t3);
        seq_t denom = 9 * (seq_t)i;
        cache[i] = numerator / denom;
    }

    cache_size = new_size;
    return cache[n];
}
