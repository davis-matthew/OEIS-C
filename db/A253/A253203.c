/*
 * A253203  The least square larger than n with same parity as n.
 * https://oeis.org/A253203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253203: the least square larger than n with the same parity as n */
#include <stdint.h>
#include <stdbool.h>
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
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t n_ = (seq_t)n;

    /* Largest integer k with k^2 <= n */
    seq_t k = floor_sqrt(n_);

    /* Candidate square root m = k+1 (always > sqrt(n)) */
    seq_t m = k + 1;

    /* Adjust m to match the parity of n */
    if ((m & 1) != (n_ & 1))
        m = m + 1;

    /* Return the square */
    return m * m;
}
