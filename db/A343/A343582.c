/*
 * A343582  a(n) = (-1)^n*n!*[x^n] exp(-3*x)/(1 - 2*x).
 * https://oeis.org/A343582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 6*(n-1)*a(n-2) - (2*n-3)*a(n-1) (n >= 3) Initial values:
 *            a(0) = 1, a(1) = 1, a(2) = 5
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache to handle arbitrary call order */
    static seq_t *cache = NULL;      /* array holding computed values */
    static idx_t max_computed = 0;   /* highest index computed in cache */

    /* Initialize cache for the first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) abort();             /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        max_computed = 2;
    }

    /* Expand cache and compute new values if needed */
    if (n > max_computed) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();         /* allocation failure */
        cache = new_cache;

        /* Compute values from (max_computed + 1) up to n */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)1;
            } else if (i == 2) {
                cache[2] = (seq_t)5;
            } else {
                seq_t term1 = (seq_t)6 * ((seq_t)(i - 1)) * cache[i - 2];
                seq_t term2 = ((seq_t)(2 * i - 3)) * cache[i - 1];
                cache[i] = term1 - term2;
            }
        }
        max_computed = n;
    }

    return cache[n];
}
