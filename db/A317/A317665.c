/*
 * A317665  Expansion of 1/Sum_{k>=0} x^(k^2).
 * https://oeis.org/A317665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(n) = - Sum_{k=1..n} [k is a perfect square] * a(n-k)
 * Range:     i8 n <= 23, i16 n <= 49, i32 n <= 98, i64 n <= 193,
 *            i128 n <= 387, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of allocated entries */
    static idx_t computed = 0;     /* last index that has been computed */

    if (cache == NULL) {
        /* First call: allocate space for at least n+1 entries */
        cache_size = n + 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 1;             /* a(0) = 1 */
        computed = 0;
    } else if (n >= cache_size) {
        /* Grow cache to accommodate entries up to n */
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    }

    /* Compute all missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 1; j * j <= i; ++j) {
            sum += cache[i - j * j];
        }
        cache[i] = -sum;
    }

    computed = n;
    return cache[n];
}
