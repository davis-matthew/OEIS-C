/*
 * A377722  n appears n^4 times.
 * https://oeis.org/A377722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that Σ_{i=1}^m i⁴ ≥ n (the integer m
 *            appears m⁴ times in the sequence)
 * Range:     u8 n <= 9785, u16 n <= 9785, u32 n <= 9785, u64 n <= 9785,
 *            u128 n <= 9785, big any n
 * Helpers:   sum_i4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sum_i4.h"

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
