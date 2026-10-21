/*
 * A259196  Number of partitions of n into six primes.
 * https://oeis.org/A259196
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of partitions of n into six primes (order disregarded)
 * Range:     u8 n <= 77, u16 n <= 399, u32 n <= 6743, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes a(n) for n ≥ 12.  It pre‑computes the DP table
   up to at least the requested n on first use and re‑computes it if a
   larger n is later requested.  The DP is based on the coin‑change
   formulation with k = 6 denominations (the primes). */

#include <stddef.h>           // For size_t
#include <stdlib.h>           // For free()
#include "../utils/build_primes_upto.h"   // build_primes_upto
#include "../utils/seq_alloc_new_array.h" // seq_alloc_new_array
/* seq_t and idx_t are defined by the build system. */

static seq_t *dp[7];           /* dp[k][s] = ways to sum to s with k primes */
static bool init_done = false;
static idx_t max_init = 0;

/* Ensure that dp tables are computed up to at least max_n. */
static void init_dp(idx_t max_n)
{
    size_t i, j;
    size_t prime_cnt;
    uint64_t *primes;

    /* Allocate fresh tables.  They are zeroed afterwards. */
    for (j = 0; j <= 6; ++j) {
        dp[j] = seq_alloc_new_array(max_n + 1);
        for (i = 0; i <= max_n; ++i)
            dp[j][i] = 0;
    }

    dp[0][0] = 1;   /* base case: one way to sum to 0 with 0 primes */

    /* Generate all primes ≤ max_n. */
    build_primes_upto(max_n, &primes, &prime_cnt);

    for (size_t p_idx = 0; p_idx < prime_cnt; ++p_idx) {
        idx_t p = (idx_t)primes[p_idx];
        for (idx_t s = p; s <= max_n; ++s) {            /* ascending to allow reuse of same prime */
            for (int k = 1; k <= 6; ++k) {
                dp[k][s] += dp[k - 1][s - p];
            }
        }
    }

    free(primes);            /* free the temporary prime array */
    init_done = true;
    max_init = max_n;
}

/* Public function required by the interface. */
seq_t a(idx_t n)
{
    /* For n < 12, the function is indifferent; return 0. */
    if (n < 12)
        return 0;

    /* Initialise or extend the DP table if necessary. */
    if (!init_done || n > max_init)
        init_dp((n > max_init ? n : max_init));

    return dp[6][n];
}
