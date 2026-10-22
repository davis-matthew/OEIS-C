/*
 * A330088  a(n) = Sum_{k=1..n} binomial(n,k) * sigma(k) * sigma(n - k + 1),
 *          where sigma = A000203.
 * https://oeis.org/A330088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * sigma(k) * sigma(n - k + 1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  sigma is the sum of divisors function (OEIS A000203) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static cache for sigma values */
static seq_t *sigma_cache = NULL;   /* array indexed 0..max_n, we use 1..max_n */
static idx_t sigma_max = 0;

/* recompute sigma array up to n (inclusive) */
static void recompute_sigma(idx_t n)
{
    seq_t *new_cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!new_cache) return;  /* out of memory; silently fail (not expected) */

    /* initialise to 0 */
    for (idx_t i = 1; i <= n; ++i) new_cache[i] = 0;

    /* sieve-like accumulation of divisors */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i)
            new_cache[j] += (seq_t)i;
    }

    free(sigma_cache);
    sigma_cache = new_cache;
    sigma_max = n;
}

/* ensure sigma values up to n are available */
static void ensure_sigma(idx_t n)
{
    if (n <= sigma_max) return;
    recompute_sigma(n);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but handle 0 safely */

    ensure_sigma(n);                /* build sigma[1..n] */

    seq_t res = 0;
    seq_t binom = (seq_t)n;         /* C(n,1) */

    for (idx_t k = 1; k <= n; ++k) {
        if (k > 1)
            binom = (binom * (seq_t)(n - k + 1)) / (seq_t)k;

        seq_t term = binom * sigma_cache[k] * sigma_cache[n - k + 1];
        res += term;
    }

    return res;
}
