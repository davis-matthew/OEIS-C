/*
 * A320394  Number of ones in binary expansion n^5.
 * https://oeis.org/A320394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = popcount(n^5)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x = pow_seq(x, 5);          /* compute n^5 */
    seq_t count = 0;
    while (x) {
        count += x & 1;
        x >>= 1;
    }
    return count;
}
