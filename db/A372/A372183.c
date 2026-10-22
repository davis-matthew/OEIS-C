/*
 * A372183  E.g.f. A(x) satisfies A(x) = exp( x * A(x)^5 / (1 - x * A(x)^2) ).
 * https://oeis.org/A372183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} (3k+2n+1)^{k-1} * binomial(n-1,k-1) /
 *            k! , derived from A(x) = exp( x * A(x)^5 / (1 - x * A(x)^2) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/* Cached values of the sequence to support arbitrary-order queries */
static seq_t *cache = NULL;        /* array of seq_t values */
static size_t cache_cap = 0;       /* allocated capacity of cache */
static idx_t  cache_len = 0;       /* number of computed terms (cache[0..cache_len-1]) */

/* Compute a single term a(n) (n > 0) using the closed form */
static seq_t compute_term(idx_t n)
{
    /* factorial of n: fac_n = n! */
    seq_t fac_n = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fac_n *= (seq_t)i;

    seq_t ratio  = fac_n;       /* a(n) / k! for current k */
    seq_t binom  = (seq_t)1;    /* C(n-1, k-1) for current k */
    seq_t sum    = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        /* base = 3k + 2n + 1 */
        idx_t base_idx = 3 * k + 2 * n + 1;
        seq_t base     = (seq_t)base_idx;

        /* pow_val = base^(k-1) */
        seq_t pow_val  = big_pow(base, (seq_t)(k - 1));

        /* term = ratio * pow_val * binom */
        seq_t term = ratio * pow_val;
        term = term * binom;
        sum += term;

        /* Prepare ratio and binom for next k, unless at the end */
        if (k < n) {
            ratio /= (seq_t)(k + 1);                 /* fac_n / (k+1)!  */
            binom = (binom * (seq_t)(n - k)) / (seq_t)k; /* C(n-1, k)       */
        }
    }
    return sum;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Ensure the cache is initialized */
    if (cache == NULL) {
        cache_cap = 1;
        cache     = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) {
            /* Allocation failed – return zero as a safe fallback */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_len = 1;
    }

    /* Return stored value if already computed */
    if (n < cache_len) {
        return cache[n];
    }

    /* Extend cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) {
            /* Realloc failed – return zero or use previous cache */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = compute_term(i);
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
