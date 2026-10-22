/*
 * A300978  Number of partitions of n into distinct parts having the same
 *          number of divisors as n.
 * https://oeis.org/A300978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 290, u32 n <= 830, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A300978: Number of partitions of n into distinct parts having the
 * same number of divisors as n.
 * Formula: a(n) = coefficient of xⁿ in ∏{ d(k) = d(n) } (1 + xᵏ).
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- divisor‑count cache ------------------------------------------------ */
static unsigned int *divcnt = NULL;   /* 1‑based divisor counts */
static size_t       divcnt_size = 0;  /* allocated size (divcnt_size-1 = max n) */

/* Fully recompute divisor counts up to 'upto' (inclusive).  This function
 * eliminates double counting by resetting all entries before the sieve.       */
static void recompute_divcnt(size_t upto)
{
    /* Ensure enough space. */
    if (divcnt_size < upto + 1) {
        unsigned int *tmp = (unsigned int *)realloc(divcnt,
                         (upto + 1) * sizeof(unsigned int));
        if (!tmp) exit(1);
        divcnt = tmp;
        divcnt_size = upto + 1;
    }
    /* reset previous counts. */
    for (size_t i = 0; i <= upto; ++i) divcnt[i] = 0;
    /* standard divisor‑count sieve. */
    for (size_t i = 1; i <= upto; ++i)
        for (size_t j = i; j <= upto; j += i)
            ++divcnt[j];
}

/* ----- sequence ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;              /* a(0) = 1 by definition */

    /* Compute divisor counts up to n. */
    recompute_divcnt((size_t)n);

    unsigned int target_d = divcnt[(size_t)n];    /* d(n) */

    /* DP array: dp[s] = #partitions of s using eligible distinct parts. */
    seq_t *dp = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
    if (!dp) exit(1);
    for (size_t i = 0; i <= (size_t)n; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;

    /* Iterate over all k with d(k)=d(n). */
    for (idx_t k = 1; k <= n; ++k) {
        if (divcnt[(size_t)k] != target_d) continue;
        /* 0/1 knapsack update: loop descending to avoid reusing k. */
        for (idx_t sum = n; sum >= k; --sum) {
            dp[(size_t)sum] += dp[(size_t)(sum - k)];
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
