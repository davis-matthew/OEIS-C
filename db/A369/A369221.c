/*
 * A369221  Number of compositions (ordered partitions) of n into prime power
 *          parts (not including 1) not greater than sqrt(n).
 * https://oeis.org/A369221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered partitions of n into prime powers *
 *            (≥2) that are ≤ √n. The parts are allowed to be any pᵏ with p
 *            prime * and k ≥ 1. The function uses dynamic programming and
 *            does not hard‑ * code any terms
 * Range:     u8 n <= 17, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root of an unsigned 64‑bit integer (truncated). */
static idx_t int_sqrt_idx(idx_t v) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= v) r++;
    while (r * r > v) r--;
    return r;
}

/* Hard‑coded list of primes up to 100 (the maximal √n for n ≤ 10 000). */
static const uint64_t _primes_upto_100[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL,
    71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL
};
static const size_t _primes_len = 25;

/* Main routine. */
seq_t a(idx_t n) {
    /* a(0) = 1 by convention. */
    if (n == 0) return (seq_t)1;

    /* Compute maximum allowed part: the integer part of sqrt(n). */
    idx_t max_part = int_sqrt_idx(n);
    if (max_part < 2) return (seq_t)0;      /* no admissible part */

    /* Build the list of prime power parts ≤ max_part. */
    seq_t parts[64];
    size_t parts_cnt = 0;
    for (size_t pi = 0; pi < _primes_len; ++pi) {
        seq_t p = _primes_upto_100[pi];
        if (p > max_part) break;
        seq_t power = p;
        while (power <= max_part) {
            parts[parts_cnt++] = power;
            power *= p;                /* next power of the same prime */
        }
    }
    if (parts_cnt == 0) return (seq_t)0;    /* should not happen for n≥4 */

    /* DP array: dp[i] = number of compositions of i. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;           /* allocation failure */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k < parts_cnt; ++k) {
            idx_t part = (idx_t)parts[k];
            if (part <= i)
                sum += dp[i - part];
        }
        dp[i] = sum;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
