/*
 * A281616  Expansion of Sum_{p prime, i>=1} x^(p^i)/(1 - x^(p^i)) /
 *          Product_{p prime, j>=1} (1 - x^(p^j)).
 * https://oeis.org/A281616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of parts in all partitions of n into
 *            prime‑power parts (excluding 1). Computation: dynamic
 *            programming over the set S = {p^k | p prime, k ≥ 1}. dp[i] –
 *            number of such partitions of i parts[i] – sum of the numbers of
 *            parts over all partitions of i For each s in S: dp[i] += dp[i...
 * Range:     u8 n <= 18, u16 n <= 49, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"

static seq_t *dp = NULL;          /* number of partitions */
static seq_t *parts = NULL;       /* total number of parts */
static idx_t *prime_powers = NULL;/* list of prime powers ≤ current limit */
static size_t pp_cnt = 0;         /* number of prime powers */
static idx_t current_limit = 0;   /* largest n for which dp/parts are valid */

/* Re‑compute dp and parts up to new_limit (inclusive) */
static void recompute(idx_t new_limit)
{
    /* Resize dp and parts arrays */
    dp     = (seq_t*)realloc(dp,     (new_limit + 1) * sizeof(seq_t));
    parts  = (seq_t*)realloc(parts,  (new_limit + 1) * sizeof(seq_t));

    /* Initialise arrays */
    for (idx_t i = 0; i <= new_limit; ++i) {
        dp[i]    = 0;
        parts[i] = 0;
    }
    dp[0]    = 1;   /* empty partition */
    parts[0] = 0;

    /* Generate primes up to new_limit */
    size_t prime_cnt = 0;
    idx_t *primes = NULL;
    build_primes(new_limit, &primes, &prime_cnt);

    /* First count how many prime powers (≤ new_limit) we will generate */
    size_t estimated_pp = 0;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];
        idx_t val = p;
        while (val <= new_limit) {
            ++estimated_pp;
            val *= p;
        }
    }

    /* Allocate or resize prime_powers array */
    if (prime_powers) free(prime_powers);
    prime_powers = (idx_t*)malloc(estimated_pp * sizeof(idx_t));
    if (!prime_powers) { /* allocation failure -- abort program */
        exit(EXIT_FAILURE);
    }

    /* Fill prime_powers array with actual values */
    pp_cnt = 0;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p  = primes[pi];
        idx_t val = p;
        while (val <= new_limit) {
            prime_powers[pp_cnt++] = val;
            val *= p;
        }
    }

    /* Dynamic programming over all prime powers */
    for (size_t ii = 0; ii < pp_cnt; ++ii) {
        idx_t s = prime_powers[ii];
        for (idx_t i = s; i <= new_limit; ++i) {
            seq_t dp_prev = dp[i - s];
            dp[i]    += dp_prev;
            parts[i] += parts[i - s] + dp_prev;
        }
    }

    free(primes);
    current_limit = new_limit;
}

/* External function requested by the interface */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* sequence is undefined for n < 1, but return 0 */
    if (n > current_limit) {
        recompute(n);
    }
    return parts[n];
}
