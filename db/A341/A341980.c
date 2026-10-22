/*
 * A341980  Number of partitions of n into 9 distinct primes (counting 1 as a
 *          prime).
 * https://oeis.org/A341980
 *
 * Offset:    78    (a(78) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into 9 distinct primes (counting 1 as
 *            a prime)
 * Range:     u8 n <= 151, u16 n <= 151, u32 n <= 151, u64 n <= 151,
 *            u128 n <= 151, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Forward declaration of the sequence function */
seq_t a(idx_t n);

/* Static helpers and state for memoisation */
static seq_t **dp = NULL;          /* DP table: dp[k][s] = number of ways to choose k primes summing to s */
static idx_t cur_limit = 0;        /* Current maximum n for which dp has been built */
static int dp_allocated = 0;       /* Flag indicating whether dp has been allocated */

/* Utility: free the previous DP table */
static void free_dp(idx_t limit)
{
    for (idx_t k = 0; k < 10; ++k)
        free(dp[k]);
    free(dp);
    dp_allocated = 0;
    cur_limit = 0;
}

/* Build DP table up to 'limit' inclusive */
static void build_dp(idx_t limit)
{
    /* Allocate DP table: 10 rows (k = 0..9), each with (limit + 1) columns */
    dp = (seq_t **)malloc(sizeof(seq_t *) * 10);
    for (idx_t k = 0; k < 10; ++k)
        dp[k] = (seq_t *)calloc((size_t)limit + 1, sizeof(seq_t));

    /* DP base case: one way to choose 0 primes summing to 0 */
    dp[0][0] = (seq_t)1;

    /* ----- Prime generation up to 'limit' ----- */
    /* 1 is treated as a prime for this sequence, but it is not produced by the sieve. */
    /* We include it explicitly later. */
    size_t sieve_limit = (size_t)limit;
    uint8_t *is_composite = (uint8_t *)calloc(sieve_limit + 1, sizeof(uint8_t));
    /* Allocate a temporary buffer for primes: the number of primes up to 10000 is < 1229, */
    /* but we allocate a bit more to be safe. */
    uint32_t *primes = (uint32_t *)malloc((sieve_limit / 2 + 10) * sizeof(uint32_t));
    size_t primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if ((idx_t)i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    free(is_composite);

    /* Prepend 1 to the list of primes */
    /* Since the order does not affect the subset-sum DP, we can simply process 1 first. */
    for (size_t pidx = 0; pidx < primes_cnt; ++pidx) {
        idx_t p = (idx_t)primes[pidx];
        /* Update DP for prime p */
        for (idx_t k = 9; k >= 1; --k) {
            for (idx_t s = limit; s >= p; --s) {
                dp[k][s] += dp[k - 1][s - p];
            }
        }
    }
    /* Process prime 1 */
    idx_t p = 1;
    for (idx_t k = 9; k >= 1; --k) {
        for (idx_t s = limit; s >= p; --s) {
            dp[k][s] += dp[k - 1][s - p];
        }
    }

    free(primes);
    dp_allocated = 1;
    cur_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!dp_allocated || n > cur_limit)
        build_dp(n);
    /* For n < 78 the DP entry will be zero (no way to sum to such small numbers). */
    return dp[9][n];
}
