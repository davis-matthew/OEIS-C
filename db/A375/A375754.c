/*
 * A375754  a(n) is the difference between the next greater square of a prime
 *          and n*(n+1)/2.
 * https://oeis.org/A375754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n)^2 - n*(n+1)/2, where p(n) is the smallest prime
 *            strictly greater than sqrt(n*(n+1)/2)
 * Range:     u8 n <= 10, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 79 terms through n = 954 (2026-11-01)
 * Helpers:   ipow_i32, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* triangular number T(n) = n*(n+1)/2 */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* largest integer k with k^2 ≤ t */
    seq_t sq = floor_sqrt(t);

    /* candidate prime must be > sqrt(t) */
    idx_t cand = (idx_t)sq + 1;

    /* find the smallest prime ≥ cand */
    while (!is_prime(cand))
        ++cand;

    /* square of that prime */
    seq_t p  = (seq_t)cand;
    seq_t p_sq = p * p;

    /* difference */
    return p_sq - t;
}
