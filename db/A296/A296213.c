/*
 * A296213  a(n) = 1 if both 1+phi(k) and 1+sigma(k) are squares, 0 otherwise.
 * https://oeis.org/A296213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 7253, u32 n <= 65593, u64 n <= 65593,
 *            u128 n <= 65593, big any n
 * Validated: standard, pass, 139 terms through n = 9893 (2026-11-01)
 * Helpers:   ipow_i32, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Check whether 1+phi(n) and 1+sigma(n) are perfect squares. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/totient_seq.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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
    /* Convert the index to a seq_t for phi calculation */
    seq_t k = (seq_t)n;

    /* Euler totient and sum-of-divisors of n */
    seq_t phi   = totient_seq(k);
    seq_t sigma = sigma_of(n);

    /* Add 1 to each */
    seq_t phi1   = phi + 1;
    seq_t sigma1 = sigma + 1;

    /* Floor square roots */
    seq_t sqrt_phi   = floor_sqrt(phi1);
    seq_t sqrt_sigma = floor_sqrt(sigma1);

    /* Both must be perfect squares */
    if (sqrt_phi * sqrt_phi == phi1 && sqrt_sigma * sqrt_sigma == sigma1)
        return 1;
    else
        return 0;
}
