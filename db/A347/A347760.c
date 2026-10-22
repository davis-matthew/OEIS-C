/*
 * A347760  Number of compositions (ordered partitions) of n into at most 5
 *          prime parts (counting 1 as a prime).
 * https://oeis.org/A347760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into at most 5 prime * parts
 *            (with 1 treated as a prime). The computation uses dynamic *
 *            programming: dp[k][s] = number of compositions of s with exactly
 *            k * parts (each part ∈ {1} ∪ primes). Then a(n) = Σ_{k=1..5}
 *            dp[k][n]
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include required utilities for building prime lists. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"

/* Static storage used for memoisation. */
static seq_t *dp[6] = {NULL};          /* dp[0]..dp[5] arrays */
static size_t maxN = 0;                /* maximum n for which dp is filled */
static uint64_t *prime_list = NULL;    /* list of primes (≥2) up to maxN  */
static size_t prime_count = 0;         /* number of primes in the list */

/* Ensure that dp arrays are computed up to at least n. */
static void ensure_capacity(idx_t n)
{
    if (n <= maxN) return;            /* already available */

    /* Rebuild prime list up to the new limit. */
    if (prime_list != NULL) free(prime_list);
    build_primes_upto((idx_t)n, &prime_list, &prime_count);

    /* Grow each dp array to size n+1. */
    for (int k = 0; k <= 5; k++) {
        seq_t *newp = (seq_t *)realloc(dp[k], ((size_t)n + 1) * sizeof(seq_t));
        if (!newp) abort();           /* allocation failure */
        dp[k] = newp;
    }

    /* Initialise dp[0][s] = 0 for new indices; set dp[0][0] = 1. */
    memset(dp[0] + maxN + 1, 0, ((size_t)n - maxN) * sizeof(seq_t));
    dp[0][0] = (seq_t)1;

    /* Compute dp values for new indices (old ones retain values). */
    for (size_t s = maxN + 1; s <= (size_t)n; s++) {
        /* k = 1 */
        seq_t sum = dp[0][s - 1];                /* part = 1 */
        for (size_t pi = 0; pi < prime_count && prime_list[pi] <= s; pi++)
            sum += dp[0][s - prime_list[pi]];
        dp[1][s] = sum;

        /* k = 2..5 */
        for (int k = 2; k <= 5; k++) {
            seq_t s2 = dp[k - 1][s - 1];          /* part = 1 */
            for (size_t pi = 0; pi < prime_count && prime_list[pi] <= s; pi++)
                s2 += dp[k - 1][s - prime_list[pi]];
            dp[k][s] = s2;
        }
    }
    maxN = (size_t)n;
}

/* Public function requested by the framework. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;  /* empty composition */
    ensure_capacity(n);
    seq_t total = 0;
    for (int k = 1; k <= 5; k++)
        total += dp[k][(size_t)n];
    return total;
}
