/*
 * A354311  Expansion of e.g.f. exp( x/2 * (exp(2 * x) - 1) ).
 * https://oeis.org/A354311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 (for n=0); a(0) = 1, a(1) = 0, a(n) = Sum_{k=2..n} k *
 *            2^(k-2) * binomial(n-1, k-1) * a(n-k)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoised values of a(n) */
static idx_t  cache_len = 0;         /* indices 0 .. cache_len-1 are already computed */
static size_t cache_cap = 0;         /* allocated capacity of 'cache' */

seq_t a(idx_t n)
{
    /* Ensure storage capacity for n+1 values */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Handle allocation failure gracefully */
            exit(EXIT_FAILURE);
        }
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }
        if (i == 1) {
            cache[1] = 0;
            continue;
        }

        seq_t sum = 0;
        seq_t C   = (seq_t)(i - 1);     /* binomial(i-1,1)   */
        seq_t pow2_local = 1;           /* 2^(0) for k=2    */

        for (idx_t k = 2; k <= i; ++k) {
            seq_t term = (seq_t)k;
            term *= pow2_local;
            term *= C;
            term *= cache[i - k];
            sum += term;

            if (k < i) {
                /* Update C for the next k: C = C * (i - k) / k   */
                C *= (seq_t)(i - k);
                C /= (seq_t)k;

                /* Advance 2^(k-2) for the next term: multiply by 2   */
                pow2_local <<= 1;
            }
        }

        cache[i] = sum;
    }

    cache_len = n + 1;   /* we now have values 0 .. n */
    return cache[n];
}
