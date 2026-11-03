/*
 * A377499  a(n) is the median of the divisors of 2n-1.
 * https://oeis.org/A377499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = median of the divisors of 2n-1 i.e. a(n) = (d +
 *            (2n-1)/d)/2 where d is the largest divisor of (2n-1) not
 *            exceeding its square root
 * Range:     u8 n <= 32, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

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
    /* u = 2n – 1 */
    seq_t u = (seq_t)n * (seq_t)2 - (seq_t)1;

    /* square root of u (floor) – needed for the divisor bound */
    seq_t sqrt_u_seq = floor_sqrt(u);
    idx_t sqrt_u = (idx_t)sqrt_u_seq;

    /* find the largest divisor d ≤ sqrt_u */
    seq_t d_seq = (seq_t)1;
    for (idx_t d = (idx_t)1; d <= sqrt_u; ++d) {
        if ((u % (seq_t)d) == (seq_t)0) {
            d_seq = (seq_t)d;
        }
    }

    /* other divisor is u/d_seq, median is their average */
    seq_t other    = u / d_seq;
    seq_t sum      = d_seq + other;
    seq_t median   = sum / (seq_t)2;

    return median;
}
