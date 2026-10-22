/*
 * A340228  a(n) is the sum of the lengths of all the segments used to draw a
 *          rectangle of height 2^(n-1) and width n divided into 2^(n-1)
 *          rectangles of unit height, in turn, divided into rectangles of
 *          unit height and lengths corresponding to the parts of the
 *          compositions of n.
 * https://oeis.org/A340228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 3*(n + 1)*2^(n-2), a(1) = 4
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)4;

    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n - 2; ++i) {
        pow2 *= (seq_t)2;
    }

    seq_t res   = (seq_t)n;
    seq_t tmp   = (seq_t)(n + 1);
    tmp *= (seq_t)3;
    tmp *= pow2;
    res += tmp;

    return res;
}
