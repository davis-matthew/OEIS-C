/*
 * A276647  Number of squares after the n-th generation in a symmetric (with
 *          45-degree angles) non-overlapping Pythagoras tree.
 * https://oeis.org/A276647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for even n: a(n) = 17*2^(n/2+1) - 2*n^2 - 10*n - 33 for odd n:
 *            a(n) = 3*2^((n+7)/2) - 2*n^2 - 10*n - 33
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 53, u64 n <= 117, u128 n <= 245,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t poly = 2 * nn * nn + 10 * nn + 33;   /* 2*n^2 + 10*n + 33 */

    if ((n & 1) == 0) {
        /* even n */
        return pow2(n / 2 + 1) * 17 - poly;
    } else {
        /* odd n */
        return pow2((n + 7) / 2) * 3 - poly;
    }
}
