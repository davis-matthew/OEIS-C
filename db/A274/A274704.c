/*
 * A274704  Exponential generating function 1/M_{4}(z^4) where M_{n}(z) is the
 *          n-th Mittag-Leffler function, nonzero coefficients only.
 * https://oeis.org/A274704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = - Σ_{k=0}^{n-1} a(k) * binomial(4*n + 1, 4*k +
 *            1) for n ≥ 1
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 4, i64 n <= 6, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build system.
   The cache stores already computed values of the sequence. */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms stored (cache[0] .. cache[cache_size-1]) */

seq_t a(idx_t n)
{
    /* Grow the cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t old_size = cache_size;
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache      = tmp;
        cache_size = new_size;

        /* Compute new terms from old_size up to n */
        for (size_t m = old_size; m < new_size; ++m) {
            if (m == 0) {
                cache[0] = 1;        /* base case */
                continue;
            }

            /* bin = binomial(4*m + 1, 1) = 4*m + 1 */
            seq_t bin = (seq_t)(4 * m + 1);
            seq_t sum = 0;

            /* Use k from 0 to m-1 */
            for (size_t k = 0; k < m; ++k) {
                /* sum += cache[k] * bin */
                sum += cache[k] * bin;

                /* Advance bin to binomial(4*m + 1, 4*(k+1) + 1) */
                seq_t num, den;

                /* Step 1 */
                num = (seq_t)(4 * m + 1 - (4 * k + 1));
                den = (seq_t)(4 * k + 2);
                bin = bin * num / den;

                /* Step 2 */
                num = (seq_t)(4 * m + 1 - (4 * k + 2));
                den = (seq_t)(4 * k + 3);
                bin = bin * num / den;

                /* Step 3 */
                num = (seq_t)(4 * m + 1 - (4 * k + 3));
                den = (seq_t)(4 * k + 4);
                bin = bin * num / den;

                /* Step 4 */
                num = (seq_t)(4 * m + 1 - (4 * k + 4));
                den = (seq_t)(4 * k + 5);
                bin = bin * num / den;
            }

            cache[m] = -sum;
        }
    }

    return cache[(size_t)n];
}
