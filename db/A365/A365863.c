/*
 * A365863  a(0) = 1; thereafter a(n) = n*Sum_{k = 0..n-1} binomial(n,
 *          k)*(-1)^(1+n+k)*a(k).
 * https://oeis.org/A365863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; thereafter a(n) = n*Sum_{k = 0..n-1} binomial(n,
 *            k)*(-1)^(1+n+k)*a(k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence by dynamic programming, storing previously computed terms in a static cache. */

#include <stdlib.h>

/* External integer types provided by the build. */
extern idx_t _some_helper(); /* dummy to make the header compile */

static seq_t *cache = NULL;     /* array of computed a(n) */
static idx_t cache_size = 0;    /* number of terms stored */

/* Returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Ensure cache is initialised with a(0) = 1. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Expand cache if necessary. */
    if (cache_size <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Compute terms from cache_size up to n. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum_even = 0;   /* Sum_{k even} C(i,k)*a(k)   */
            seq_t sum_odd  = 0;   /* Sum_{k odd }  C(i,k)*a(k)   */
            seq_t C = 1;          /* Current binomial coefficient C(i,0) */

            for (idx_t k = 0; k < i; ++k) {
                seq_t term = C * cache[k];
                if (k & 1)            /* k is odd */
                    sum_odd += term;
                else
                    sum_even += term;

                /* Update binomial coefficient: C(i,k+1) = C(i,k) * (i-k)/(k+1) */
                idx_t numer = i - k;
                C = C * (seq_t)numer / (k + 1);
            }

            seq_t diff;
            if (sum_even >= sum_odd)
                diff = sum_even - sum_odd;
            else
                diff = sum_odd - sum_even;

            cache[i] = (seq_t)i * diff;
        }

        cache_size = new_cap;
    }

    return cache[n];
}
