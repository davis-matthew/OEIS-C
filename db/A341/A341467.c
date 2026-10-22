/*
 * A341467  Number of partitions of n into 8 distinct nonprime parts.
 * https://oeis.org/A341467
 *
 * Offset:    64    (a(64) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 118, u32 n <= 118, u64 n <= 118,
 *            u128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into 8 distinct nonprime parts.
   The function builds the DP table from scratch whenever a larger
   indexing value n is requested.  The table dp[k][s] counts
   partitions of s into exactly k distinct nonprime integers.  
   Only the value dp[8][n] is returned. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* forwards: external types defined by the build system */
extern seq_t a(idx_t n);

/* internal state */
static seq_t **dp = NULL;         /* dp[k][s]   k = 0..8, s = 0..cur_n */
static idx_t cur_n = 0;           /* maximum n for which dp is valid */
static bool *isprime = NULL;      /* sieve up to cur_n */

/* free previously allocated memory */
static void free_dp(void)
{
    if (dp) {
        for (int k = 0; k <= 8; ++k)
            free(dp[k]);
        free(dp);
        dp = NULL;
    }
    free(isprime);
    isprime = NULL;
    cur_n = 0;
}

/* rebuild dp table for all n up to new_n */
static void rebuild_dp(idx_t new_n)
{
    /* free old allocation */
    free_dp();

    /* allocate memory for dp rows */
    dp = (seq_t**)calloc(9, sizeof(seq_t*));
    if (!dp) exit(1);
    for (int k = 0; k <= 8; ++k) {
        dp[k] = (seq_t*)calloc((size_t)new_n + 1, sizeof(seq_t));
        if (!dp[k]) exit(1);
    }
    dp[0][0] = (seq_t)1;  /* base case: 0 parts sum to 0 */

    /* build sieve of primes up to new_n */
    isprime = (bool*)calloc((size_t)new_n + 1, sizeof(bool));
    if (!isprime) exit(1);
    for (idx_t i = 2; i <= new_n; ++i) isprime[i] = true;
    for (idx_t i = 2; i * i <= new_n; ++i) {
        if (isprime[i]) {
            for (idx_t j = i * i; j <= new_n; j += i)
                isprime[j] = false;
        }
    }

    /* DP fill: for each nonprime number m, update distinct partitions */
    for (idx_t m = 1; m <= new_n; ++m) {
        if (!isprime[m]) {
            for (int k = 8; k >= 1; --k) {
                for (idx_t s = m; s <= new_n; ++s) {
                    dp[k][s] += dp[k - 1][s - m];
                }
            }
        }
    }
    cur_n = new_n;
}

seq_t a(idx_t n)
{
    if (n < 64) return (seq_t)0;          /* offset 64 */
    if (n > cur_n) rebuild_dp(n);         /* rebuild up to requested n */
    return dp[8][n];
}
