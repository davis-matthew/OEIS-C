/*
 * A320787  Number of multisets of exactly two partitions of positive integers
 *          into distinct parts with total sum of parts equal to n.
 * https://oeis.org/A320787
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( Σ_{i=1}^{n-1} p(i)*p(n-i) + p(n/2) ) / 2 * where p(k)
 *            is the number of partitions of k into distinct parts
 * Range:     u8 n <= 16, u16 n <= 41, u32 n <= 121, u64 n <= 401,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a dynamic programming table for p(k) and
   128‑bit intermediate arithmetic to avoid overflow on 64‑bit builds. */

#include <stdlib.h>

/* Global cache for partition counts into distinct parts. */
static seq_t *dp = NULL;
static idx_t dp_max = 0;

/* Ensure that dp[0..n] contain the correct values.
   We recompute the entire table from scratch whenever a larger n is requested. */
static void ensure_dp(idx_t n)
{
    if (n <= dp_max) return;

    /* Allocate new array.  (idx_t may be 64‑bit, but n is small in practice.) */
    idx_t new_len = n + 1;
    seq_t *newdp = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
    if (!newdp) return;                /* memory allocation failed – leave dp unchanged */

    /* Initialise to zero and set dp[0] = 1 (empty partition). */
    for (idx_t i = 0; i < new_len; ++i) newdp[i] = 0;
    newdp[0] = 1;

    /* Classic DP for partitions into distinct parts:
       for each part size j, update all sums t ≥ j in decreasing order. */
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t t = n; t >= part; --t) {
            newdp[t] += newdp[t - part];
        }
    }

    /* Replace the old array. */
    free(dp);
    dp = newdp;
    dp_max = n;
}

/* Main sequence function.  idx_t n is indexed with offset 2,
   i.e. a(2) is the first term. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* behavior below the offset is irrelevant */

    /* Compute partition counts up to n. */
    ensure_dp(n);

    /* Use 128‑bit unsigned integer for intermediate arithmetic. */
    typedef unsigned __int128 uint128_t;
    uint128_t total = 0;

    for (idx_t i = 1; i < n; ++i) {
        uint128_t prod = (uint128_t)dp[i] * (uint128_t)dp[n - i];
        total += prod;
    }

    /* Add correction for the diagonal when n is even. */
    if ((n & 1) == 0) {
        total += (uint128_t)dp[n / 2];
    }

    /* The result is (total + extra) / 2. */
    uint128_t result = total >> 1;        /* division by two */
    return (seq_t)result;
}
