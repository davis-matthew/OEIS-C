/*
 * A341137  Number of ways to write n as an ordered sum of 8 prime powers
 *          (including 1).
 * https://oeis.org/A341137
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = number of ordered sums of 8 prime powers (including 1). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Cached results up to the largest n that has been computed. */
static seq_t *cache       = NULL;
static idx_t cached_limit = 0;

/* List of allowed part sizes (1 plus all prime powers ≤ limit). */
static uint64_t *prime_powers = NULL;
static size_t pp_cnt  = 0, pp_cap = 0;

/* DP arrays for the 8 convolutions.  dp_prev holds the
   counts after the previous part; dp_curr is reused each iteration. */
static seq_t *dp_prev = NULL, *dp_curr = NULL;
static size_t dp_cap = 0;

/* Recompute the sequence up to `limit`.  Every call that asks for a
   value larger than `cached_limit` triggers this routine. */
static void recompute(idx_t limit)
{
    /* ------------------------------------------------------------------
       Build the list of allowed part sizes (1 and all prime powers ≤ limit).
       ------------------------------------------------------------------ */
    if (prime_powers) {
        free(prime_powers);
        prime_powers = NULL;
    }
    pp_cnt = 0;
    pp_cap = 0;

    /* Fetch all primes up to limit. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    /* Start with 1 (p^0). */
    pp_cap = 16;
    prime_powers = (uint64_t*)malloc(pp_cap * sizeof(uint64_t));
    prime_powers[pp_cnt++] = 1ULL;

    /* Generate prime powers for each prime. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t power = p;
        while (power <= (uint64_t)limit) {
            if (pp_cnt == pp_cap) {
                pp_cap <<= 1;
                prime_powers = (uint64_t*)realloc(prime_powers,
                                                  pp_cap * sizeof(uint64_t));
            }
            prime_powers[pp_cnt++] = power;
            if (power > UINT64_MAX / p) break;  /* avoid overflow */
            power *= p;
        }
    }
    free(primes);

    /* ------------------------------------------------------------------
       Allocate / resize DP arrays.
       ------------------------------------------------------------------ */
    if (dp_cap < (size_t)(limit + 1)) {
        free(dp_prev);
        free(dp_curr);
        dp_prev = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
        dp_curr = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
        dp_cap  = limit + 1;
    }

    /* ------------------------------------------------------------------
       Perform 8 convolutions (one per part of the 8‑tuple).
       ------------------------------------------------------------------ */
    for (idx_t i = 0; i <= limit; ++i) dp_prev[i] = 0;
    dp_prev[0] = 1;            /* one way to achieve sum 0 with 0 parts */

    for (int part = 0; part < 8; ++part) {
        for (idx_t i = 0; i <= limit; ++i) dp_curr[i] = 0;
        for (size_t pi = 0; pi < pp_cnt; ++pi) {
            idx_t v = (idx_t)prime_powers[pi];
            for (idx_t s = v; s <= limit; ++s) {
                dp_curr[s] += dp_prev[s - v];
            }
        }
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    /* ------------------------------------------------------------------
       Store the computed values in the cache array.
       ------------------------------------------------------------------ */
    if (cache) free(cache);
    cache = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    /* Sequence is defined from n = 8 onward; values below 8 are set to 0. */
    for (idx_t i = 0; i < 8 && i <= limit; ++i) cache[i] = 0;
    for (idx_t i = 8; i <= limit; ++i) cache[i] = dp_prev[i];
    cached_limit = limit;
}

/* External function required by the specification. */
seq_t a(idx_t n)
{
    if (n < 8) return 0;               /* undefined below the offset – return 0 */
    if (n > cached_limit) recompute(n);
    return cache[n];
}
