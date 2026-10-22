/*
 * A365567  Expansion of e.g.f. 1 / (5 - 4 * exp(x))^(3/4).
 * https://oeis.org/A365567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (4 - k/n) * binomial(n,k) * a(n-k)
 *            (equivalently, a(0)=1 and the sum is exact with division by n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoised values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure that cache can hold indices up to n. New slots are initialised to 0. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (cache == NULL) {
            /* Unrecoverable allocation failure */
            exit(EXIT_FAILURE);
        }
        for (idx_t i = 0; i <= n; ++i) cache[i] = (seq_t)0;
        cache_size = n;
    } else if (cache_size < n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        for (idx_t i = cache_size + 1; i <= n; ++i) cache[i] = (seq_t)0;
        cache_size = n;
    }

    /* initialise the base case if not already done */
    if (cache[0] == (seq_t)0) cache[0] = (seq_t)1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* If value already computed, return it */
    if (cache[n] != (seq_t)0) return cache[n];

    /* Compute all missing values up to n in increasing order */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;   /* already computed */

        seq_t sum_num = (seq_t)0;          /* numerator of the sum */
        seq_t binom = (seq_t)1;            /* C(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* Update binomial coefficient: C(i,k) = C(i,k-1) * (i-k+1)/k */
            binom = binom * (seq_t)(i - k + 1) / k;

            /* term = (4*i - k) * binom * a(i-k) */
            seq_t term = binom * cache[i - k];
            term *= (seq_t)(4 * i - k);
            sum_num += term;
        }

        /* a(i) = sum_num / i   (division exact) */
        cache[i] = sum_num / (seq_t)i;
    }

    return cache[n];
}
