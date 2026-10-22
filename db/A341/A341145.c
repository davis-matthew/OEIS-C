/*
 * A341145  Number of partitions of n into 8 distinct prime powers (including
 *          1).
 * https://oeis.org/A341145
 *
 * Offset:    39    (a(39) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   build_primes_small_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A341145: number of partitions of n into 8 distinct prime powers (including 1) */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_small_n.h"

/* DP table: dp[9][maxN+1] (maxN is the maximal n computed so far) */
static seq_t **dp = NULL;
static idx_t maxN = 0;

/* recompute DP table up to N (inclusive) */
static void recompute(idx_t N)
{
    /* free existing table */
    if (dp) {
        for (size_t i = 0; i < 9; ++i) free(dp[i]);
        free(dp);
        dp = NULL;
    }

    /* allocate new table */
    dp = (seq_t**)malloc(9 * sizeof(seq_t*));
    if (!dp) { maxN = 0; return; }               /* allocation failed */
    for (size_t i = 0; i < 9; ++i)
        dp[i] = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));

    /* list of primes up to N */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_small_n(N, &primes, &prime_cnt);
    if (!primes) { free(dp); dp = NULL; maxN = 0; return; }

    /* DP initialization */
    dp[0][0] = 1;  /* one way to sum to 0 using 0 items */

    /* include the item 1 (p^0) */
    seq_t val = 1;
    idx_t vi = (idx_t)val;
    for (int c = 8; c >= 1; --c) {
        for (idx_t s = N; s >= vi; --s) {
            dp[c][s] += dp[c-1][s-vi];
        }
    }

    /* process all prime powers p^k (p prime, k>=1) */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        seq_t power = (seq_t)p;                    /* start with p^1 */
        while ((seq_t)power <= (seq_t)N) {
            idx_t v = (idx_t)power;
            for (int c = 8; c >= 1; --c) {
                for (idx_t s = N; s >= v; --s) {
                    dp[c][s] += dp[c-1][s-v];
                }
            }
            /* next power p^(k+1) */
            if (power > (seq_t)(N / p)) break;     /* would overflow N */
            power *= (seq_t)p;
        }
    }

    free(primes);
    maxN = N;
}

/* public function */
seq_t a(idx_t n)
{
    if (n < 39) return 0;                  /* undefined below offset */
    if (n > maxN) recompute(n);
    return dp[8][n];
}
