/*
 * A381343  Expansion of e.g.f. exp( sin(sqrt(2)*x) / sqrt(2) ).
 * https://oeis.org/A381343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = Σ_{k=0..⌊(n-1)/2⌋} (-2)^k * binomial(n-1, 2k) * a(n-2k-1)  */
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t      *cache      = NULL;   /* memoisation array */
    static idx_t       cache_cap  = 0;      /* allocated capacity */
    static idx_t       max_computed = 0;    /* highest n already computed */

    /* initialisation */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        max_computed = 0;
    }

    /* compute up to n if necessary */
    if (n > max_computed) {
        /* grow capacity if needed */
        if ((size_t)(n + 1) > cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap : 1;
            while (new_cap <= n) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t sum    = 0;
            seq_t factor = 1;                      /* (-2)^k */
            idx_t maxk   = (i - 1) >> 1;           /* floor((i-1)/2) */

            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t j   = k << 1;                     /* 2*k */
                seq_t bin = binomial(i - 1, j);
                seq_t prod = factor * bin * cache[i - 1 - j];
                sum += prod;
                factor = factor * ((seq_t)-2);          /* next power of -2 */
            }
            cache[i] = sum;
        }
        max_computed = n;
    }

    return cache[n];
}
