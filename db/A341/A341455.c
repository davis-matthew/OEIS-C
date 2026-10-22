/*
 * A341455  Number of partitions of n into 8 nonprime parts.
 * https://oeis.org/A341455
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of partitions of n into exactly 8 nonprime positive parts. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build environment.  */

/* Static tables for dynamic programming.  dp[k][s] holds the number of
   partitions of s into k nonprime parts.  k ranges from 0 to 8.  */
static seq_t *dp[9] = {NULL};
static idx_t maxComputed = 0;   /* maximum n for which dp tables are built  */

/* Helper to free a dp table.  */
static void free_dp(void) {
    for (int k = 0; k <= 8; ++k) {
        if (dp[k]) {
            free(dp[k]);
            dp[k] = NULL;
        }
    }
    maxComputed = 0;
}

/* Build sieve of primes up to `limit`.  The array `prime` is allocated
   with size limit+1 and will be true for primes, false otherwise.  */
static void build_prime_sieve(idx_t limit, bool **prime) {
    *prime = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!*prime) exit(1);
    for (idx_t i = 2; i <= limit; ++i) (*prime)[i] = true;
    for (idx_t p = 2; (idx_t)(p * p) <= limit; ++p) {
        if ((*prime)[p]) {
            for (idx_t mult = p * p; mult <= limit; mult += p)
                (*prime)[mult] = false;
        }
    }
}

/* Compute dp tables up to `limit`.  */
static void compute_dp(idx_t limit) {
    /* Allocate new tables, freeing old ones if any. */
    for (int k = 0; k <= 8; ++k) {
        free(dp[k]);
        dp[k] = (seq_t*)calloc((size_t)(limit + 1), sizeof(seq_t));
        if (!dp[k]) exit(1);
    }

    /* Sieve primes up to limit.  */
    bool *prime = NULL;
    build_prime_sieve(limit, &prime);

    dp[0][0] = 1;  /* one way to partition 0 into 0 parts */

    /* Dynamic programming over allowed part sizes.  */
    for (idx_t v = 1; v <= limit; ++v) {
        if (prime[v]) continue;   /* skip prime parts */
        for (int k = 1; k <= 8; ++k) {
            for (idx_t s = v; s <= limit; ++s) {
                dp[k][s] += dp[k-1][s - v];
            }
        }
    }

    free(prime);
    maxComputed = limit;
}

/* Public function required by the OEIS harness.  */
seq_t a(idx_t n) {
    if (n < 8) return 0;  /* offset 8 */
    if (!dp[0] || n > maxComputed) {
        compute_dp(n);
    }
    return dp[8][n];
}
