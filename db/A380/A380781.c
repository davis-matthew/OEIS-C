/*
 * A380781  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x * (1 +
 *          x)^2) / (1 + x)^2 ).
 * https://oeis.org/A380781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+1)^(k-1) * binomial(2*n+2*k+2,
 *            n-k)/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the sum directly using integer arithmetic.
   It uses a static factorial cache to keep the cost per call to O(n).  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"   /* Provides seq_t binomial(idx_t n, idx_t k); */

static seq_t *fact_cache = NULL;   /* Cache of factorials */
static size_t fact_cap   = 0;      /* Current capacity of the cache */

seq_t a(idx_t n)
{
    /* Ensure factorial cache is big enough and filled up to n */
    if (fact_cap <= (size_t)n+1) {
        size_t new_cap = (n+1) * 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;    /* Out of memory; caller should avoid this */
        if (fact_cache) free(fact_cache);
        fact_cache = new_cache;
        fact_cap = new_cap;
    }
    fact_cache[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact_cache[i] = fact_cache[i-1] * (seq_t)i;
    }
    seq_t fac_n = fact_cache[n];

    /* Compute the k = 0 term: binom(2n+2, n) * n! / (n+1) */
    seq_t bin0 = binomial(2*n + 2, n);
    seq_t term0 = bin0 * fac_n / ((seq_t)(n + 1));
    seq_t sum = term0;

    /* Iterate over k = 1 .. n */
    seq_t pow_n1 = (seq_t)1;          /* (n+1)^(k-1) */
    for (idx_t k = 1; k <= n; ++k) {
        if (k > 1) pow_n1 *= (seq_t)(n + 1);   /* Update power for k >= 2 */

        seq_t bin = binomial(2*n + 2*k + 2, n - k);
        seq_t fac_ratio = fac_n / fact_cache[k];  /* n! / k! , integer division */
        seq_t term = bin * fac_ratio * pow_n1;
        sum += term;
    }

    return sum;
}
