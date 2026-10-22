/*
 * A351734  Expansion of e.g.f. exp( 3 * x * (exp(x) - 1) ).
 * https://oeis.org/A351734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} 3^k * S(n-k,k)/(n-k)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n≥1
 *   a(n) = Σ_{k=2..n} C(n-1, k-1) * 3k * a(n-k)
 *   (C is the binomial coefficient) */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* memoised values of a(n)          */
static size_t cache_size = 0;        /* number of computed terms         */

seq_t a(idx_t n)
{
    /* If a larger n is requested, grow the cache and compute missing terms */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – terminate the program gracefully */
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        /* Compute values from the first missing index up to n */
        for (size_t idx = cache_size; idx < new_size; ++idx) {
            if (idx == 0) {
                cache[idx] = 1;                /* a(0)=1 */
                continue;
            }

            seq_t sum = 0;
            /* binom corresponds to C(idx-1, k-1) for the current k */
            seq_t binom = (seq_t)(idx - 1);

            for (idx_t k = 2; k <= (idx_t)idx; ++k) {
                /* term = binom * (3*k) * a[idx-k] */
                seq_t term = binom * (seq_t)(3 * k) * cache[(size_t)(idx - k)];
                sum += term;

                /* Prepare binom for the next k:
                 * C(idx-1, k) = C(idx-1, k-1) * (idx-k) / k
                 */
                if (k < (idx_t)idx) {
                    binom = binom * (seq_t)(idx - k) / (seq_t)k;
                }
            }
            cache[idx] = sum;
        }

        cache_size = new_size;
    }

    return cache[(size_t)n];
}
