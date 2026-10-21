/*
 * A286225  Number of compositions (ordered partitions) of n into parts with
 *          an even number of distinct prime divisors.
 * https://oeis.org/A286225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered partitions (compositions) of n where
 *            each part has an even number of distinct prime divisors.
 *            Computation: dynamic programming with dp[0] = 1 and dp[i] =
 *            Σ_{k=1..i, evenΩ(k)} dp[i-k]
 * Range:     u8 n <= 21, u16 n <= 39, u32 n <= 75, u64 n <= 148,
 *            u128 n <= 292, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static seq_t *dp = NULL;          /* dp[i] = a(i) */
static size_t dp_cap = 0;         /* number of entries stored (dp_cap = max_index + 1) */

static uint8_t *distinct = NULL;  /* distinct[i] = number of distinct prime factors of i */
static size_t distinct_cap = 0;   /* current capacity of distinct (== distinct_cap if >0) */

/* list of primes up to current limit */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_cap = 0;

/* Ensure that primes[0..primes_cnt‑1] contains all primes <= n  */
static void ensure_primes(idx_t n)
{
    if (primes_cap >= n)
        return;
    if (primes != NULL)
        free(primes);
    build_primes_upto(n, &primes, &primes_cnt);
    primes_cap = n;
}

/* Compute the number of distinct prime divisors for all numbers up to n  */
static void ensure_distinct(idx_t n)
{
    if (distinct_cap > n)          /* already large enough */
        return;

    if (distinct != NULL)
        free(distinct);

    /* allocate array of n+1 counts (index 0 unused) */
    distinct = (uint8_t *)calloc((size_t)(n + 1), sizeof(uint8_t));
    if (distinct == NULL)
        exit(EXIT_FAILURE);

    ensure_primes(n);

    /* Every prime contributes to all its multiples */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        for (uint64_t m = p; m <= (uint64_t)n; m += p)
            distinct[(size_t)m]++;        /* increment count for each distinct prime */
    }

    distinct_cap = n + 1;
}

/* Compute a(i) for all i ≤ n using dynamic programming */
static void ensure_dp(idx_t n)
{
    if (dp_cap > n)          /* already computed up to n */
        return;

    if (dp_cap == 0) {       /* first call */
        dp = (seq_t *)malloc(sizeof(seq_t));
        if (dp == NULL)
            exit(EXIT_FAILURE);
        dp[0] = (seq_t)1;
        dp_cap = 1;
    }

    dp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
    if (dp == NULL)
        exit(EXIT_FAILURE);

    for (idx_t i = dp_cap; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            if ((distinct[(size_t)k] & 1) == 0)   /* even number of distinct primes */
                sum += dp[i - k];
        }
        dp[i] = sum;
    }

    dp_cap = n + 1;
}

/* External sequence function */
seq_t a(idx_t n)
{
    ensure_distinct(n);
    ensure_dp(n);
    return dp[n];
}
