/*
 * A277465  Expansion of e.g.f. log(1+x)/(1 + LambertW(-x)).
 * https://oeis.org/A277465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (-1)^(n+1)*(n-1)! + Σ_{j=1}^{n-1} a(j)*C(n,j)*(n-j)^(n-j-1)
 * (OEIS A277465 – expansion of e.g.f. log(1+x)/(1+LambertW(-x)))
 * Initial values: a(0)=0, a(1)=1
 */

#include <stdlib.h>                /* for abort, realloc */
#include <stdint.h>                /* for uint64_t if needed */

#include "../utils/binomial.h"     /* provides seq_t binomial(idx_t n, idx_t k); */
#include "../utils/big_pow.h"      /* provides seq_t big_pow(seq_t base, seq_t exp); */

/* External linkage as required by the build system */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Static memoization arrays */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t max_n_computed = 0;
    static seq_t fact_prev = (seq_t)1; /* (k-2)! when computing k */

    /* Ensure cache has enough space for index 'n' */
    if (cache_size <= n) {
        size_t new_size = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise the first two entries if not already done */
    if (max_n_computed < 1) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_n_computed = 1;
    }

    /* Compute all terms up to 'n' that are still missing */
    for (idx_t k = max_n_computed + 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;

        /* Σ_{j=1}^{k-1} a(j)*C(k,j)*(k-j)^(k-j-1) */
        for (idx_t j = 1; j < k; ++j) {
            seq_t bin = binomial(k, j);
            seq_t pow_part = big_pow((seq_t)(k - j),
                                     (seq_t)(k - j - 1));
            seq_t term = cache[j];
            term *= bin;
            term *= pow_part;
            sum += term;
        }

        /* Compute (k-1)! using the running factorial */
        seq_t fact_kminus1 = fact_prev * (seq_t)(k - 1);
        fact_prev = fact_kminus1;          /* update for next iteration */

        /* Add or subtract the alternating factorial term */
        if (((k + 1) & 1ULL) == 1ULL)      /* (k+1) odd → subtract */
            cache[k] = sum - fact_kminus1;
        else                               /* (k+1) even → add */
            cache[k] = sum + fact_kminus1;
    }

    max_n_computed = n;
    return cache[n];
}
