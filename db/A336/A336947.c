/*
 * A336947  Expansion of e.g.f. 1 / (exp(-2*x) - x).
 * https://oeis.org/A336947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (2 * (n-k+1))^k / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive computation: a(0) = 1, a(1) = 3 */
/* a(n) = 3*n*a(n-1) - sum_{k=2..n} binom(n,k) * (-2)^k * a(n-k) */
/* The implementation uses a dynamic cache and evaluates the sum without
   negative intermediates by separating even and odd k contributions. */

#include <stdlib.h>

/* Static cache for memoised results */
static seq_t *cache = NULL;      /* pointer to cached values */
static size_t cache_sz = 0;     /* number of values currently cached (cache[0..cache_sz-1]) */
static size_t cache_cap = 0;    /* capacity of the allocated cache */

/* Ensure that values a(0) .. a(n) are computed and stored in cache */
static void ensure_cache(idx_t n)
{
    /* Allocate or grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;            /* double until sufficient */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise base values if not already present */
    if (cache_sz == 0) {
        cache[0] = 1;
        cache_sz = 1;
    }
    if (cache_sz == 1) {
        cache[1] = 3;
        cache_sz = 2;
    }

    /* Compute missing entries */
    for (size_t idx = cache_sz; idx <= (size_t)n; ++idx) {
        const idx_t i = (idx_t)idx;          /* current index to compute */
        seq_t sum_pos = 0;   /* sum over even k >= 2 of binom*2^k*a(i-k)   */
        seq_t sum_neg = 0;   /* sum over odd  k >= 3 of binom*2^k*a(i-k)   */

        seq_t binom = 1;     /* binom(i,k) initialised to 1 (k=0) */
        seq_t pow2 = 1;      /* 2^k, initialised to 1 (k=0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* Update binom(i,k) = binom(i,k-1) * (i-k+1) / k  */
            binom = binom * (i - k + 1) / k;
            pow2 = pow2 * 2;                          /* 2^k */

            if (k >= 2) {
                seq_t term = binom * pow2 * cache[i - k];
                if (k & 1)   /* odd k */
                    sum_neg += term;
                else          /* even k */
                    sum_pos += term;
            }
        }

        /* a(i) = 3*i*a(i-1) - sum_pos + sum_neg */
        seq_t three_i_a_prev = 3 * (seq_t)i * cache[i - 1];
        seq_t temp = three_i_a_prev + sum_neg;
        seq_t result = temp - sum_pos;

        cache[i] = result;    /* store computed value */
    }

    cache_sz = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
