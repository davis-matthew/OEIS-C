/*
 * A377722  n appears n^4 times.
 * https://oeis.org/A377722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that Σ_{i=1}^m i⁴ ≥ n (the integer m
 *            appears m⁴ times in the sequence)
 * Range:     u8 n <= 15333, u16 n <= 15333, u32 n <= 15333, u64 n <= 15333,
 *            u128 n <= 15333, big any n
 * Validated: standard, pass, 103 terms through n = 9785 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128 sum_i4(idx_t m)
{
    /* Compute m*(m+1)*(2*m+1)*(3*m*m+3*m-1)/30 using 128-bit
       intermediate to avoid overflow. */
    unsigned __int128 mm = m;
    unsigned __int128 term = mm * (mm + 1) * (2 * mm + 1);
    unsigned __int128 poly = 3 * mm * mm + 3 * mm - 1;
    term = term * poly / 30;
    return term;
}

seq_t a(idx_t n)
{
    /* Binary search for the smallest m with sum_i4(m) ≥ n. */
    idx_t low = 1;
    idx_t high = 1;

    /* Grow high until it is large enough. The value of m grows as n^(1/5),
       so this loop executes few iterations (≤ ~20). */
    while (sum_i4(high) < (unsigned __int128)n)
        high <<= 1U;

    while (low < high) {
        idx_t mid = low + (high - low) / 2U;
        if (sum_i4(mid) >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1U;
    }
    return (seq_t)low;
}
