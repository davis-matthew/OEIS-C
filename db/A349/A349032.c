/*
 * A349032  G.f. A(x) satisfies: A(x) = 1 / (1 - x - x * A(-2*x)).
 * https://oeis.org/A349032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; For n > 0: a(n) = a(n-1) + Sum_{k=0..n-1} (-2)^k *
 *            a(k) * a(n-k-1) Implements the recurrence directly with
 *            memoisation
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 8, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoised terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t capacity = 0;

    /* initialise cache for n = 0 */
    if (cache == NULL) {
        capacity = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!cache) exit(1);
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_size = 1;
    }

    /* extend storage if needed */
    if (n + 1 > capacity) {
        capacity = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * capacity);
        if (!tmp) { free(cache); exit(1); }
        cache = tmp;
    }

    /* return already computed value if possible */
    if (cache_size > n) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        seq_t pow = (seq_t)1;          /* (-2)^0 initially */
        for (idx_t k = 0; k <= i - 1; ++k) {
            seq_t term = pow * cache[k];
            term *= cache[i - 1 - k];
            sum += term;
            pow *= (seq_t)-2;           /* advance exponent */
        }
        cache[i] = cache[i - 1] + sum;
    }

    cache_size = n + 1;
    return cache[n];
}
