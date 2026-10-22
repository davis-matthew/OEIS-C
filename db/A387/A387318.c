/*
 * A387318  a(n) is the maximum number of water-pouring moves needed to reach
 *          (1, 1, ..., 1) from any valid initial configuration for n bottles.
 * https://oeis.org/A387318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = floor(4*n/3)-1 for n ≠ 3,4,6 a(3) = 2, a(4) = 3, a(6) = 6
 *            (these three values violate the above by 1)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    // Compute floor(4*n/3) safely in seq_t
    seq_t f = ((seq_t)n * 4) / 3;

    // Special cases for n = 3, 4, 6
    if (n == 3 || n == 4 || n == 6)
        return f - 2;
    else
        return f - 1;
}
