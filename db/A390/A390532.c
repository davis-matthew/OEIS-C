/*
 * A390532  Sequences A001477 and A000079 interlaced.
 * https://oeis.org/A390532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/2 if n is even, a(n) = 2^((n-1)/2) if n is odd
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0) {
        return (seq_t)(n / 2);
    } else {
        return pow2((idx_t)((n - 1) / 2));
    }
}
