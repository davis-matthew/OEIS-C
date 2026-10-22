/*
 * A382034  E.g.f. A(x) satisfies A(x) = exp(x*B(x*A(x))^4), where B(x) = 1 +
 *          x*B(x)^4 is the g.f. of A002293.
 * https://oeis.org/A382034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{k=0..n-1} (k+1)^(n-k-1) *
 *            binomial(4*n,k)/(n-k-1)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1                                                                   */

#include <stdlib.h>
#include "../utils/big_pow.h"

static seq_t *cache = NULL;     /* cache of computed terms */
static idx_t cache_cap   = 0;   /* allocated capacity */
static idx_t cache_size  = 0;   /* number of valid entries in cache */

/* Ensure that a(0) .. a(n) are computed and stored in cache */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    /* Grow cache if necessary */
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);
    cache = new_cache;
    cache_cap = new_cap;

    /* Compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            /* a(0) = 1 */
            val = (seq_t)1;
        } else {
            /* Compute a(i) using the given formula */
            idx_t N = i;
            seq_t sum = 0;

            /* C(4N, 0) = 1  and  product = 1  at the start */
            seq_t binom   = (seq_t)1;  /* current binomial coefficient C(4N, k) */
            seq_t product = (seq_t)1;  /* (N-1)!/(N-k-1)!  for current k */

            for (idx_t k = 0; k < N; ++k) {
                /* (k+1)^(N-k-1) */
                seq_t pow_term = big_pow((seq_t)(k + 1), (seq_t)(N - k - 1));

                /* term = binom * pow_term * product */
                seq_t term = binom * pow_term;
                term = term * product;
                sum += term;

                /* Prepare binom and product for next k, if any */
                if (k + 1 < N) {
                    /* Binomial: C(4N, k+1) = C(4N, k) * (4N-k)/(k+1) */
                    seq_t tmp = binom * (seq_t)(4 * N - k);
                    binom = tmp / (seq_t)(k + 1);

                    /* Product: multiply by (N - k - 1) */
                    product *= (seq_t)(N - k - 1);
                }
            }
            val = sum;
        }
        cache[i] = val;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
