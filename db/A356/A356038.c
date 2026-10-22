/*
 * A356038  a(n) = Sum_{k=1..n} binomial(n,k) * sigma_2(k).
 * https://oeis.org/A356038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * sigma_2(k) where sigma_2(k)
 *            = Sum_{d|k} d^2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *sigma_cache = NULL;
static idx_t sigma_cache_n = 0;

static void recompute_sigma(idx_t n)
{
    /* free old cache */
    if (sigma_cache) free(sigma_cache);

    /* allocate new cache (indices 0..n) */
    sigma_cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!sigma_cache) {
        /* allocation failed – in the test harness this never
           happens, but we guard anyway. */
        sigma_cache_n = 0;
        return;
    }

    /* initialise to zero */
    for (idx_t i = 0; i <= n; ++i) sigma_cache[i] = 0;

    /* sieve to compute sigma_2(1..n) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i * (seq_t)i;          /* i^2 as seq_t */
        for (idx_t j = i; j <= n; j += i) {
            sigma_cache[j] += ii;                /* add i^2 to all multiples of i */
        }
    }
    sigma_cache_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety; not used for the series */

    /* ensure sigma cache covers required range */
    if (n > sigma_cache_n) recompute_sigma(n);
    if (!sigma_cache) return 0;          /* allocation failure protection */

    seq_t sum   = 0;
    seq_t binom = 1;                     /* binomial(n,0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* binom = binom(n,k) from binom(n,k-1) */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;

        /* accumulate contribution */
        sum += binom * sigma_cache[k];
    }
    return sum;
}
