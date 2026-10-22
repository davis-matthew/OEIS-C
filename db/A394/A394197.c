/*
 * A394197  Number of squares reachable in n (but no fewer) leaps of an
 *          antelope (the fairy chess piece) on an infinite chessboard.
 * https://oeis.org/A394197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 124*n - 156 for n >= 12 a(0..11) = {1, 8, 32, 88, 192,
 *            360, 608, 832, 960, 1040, 1112, 1212}
 * Range:     u8 n <= 4, u16 n <= 529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* first 12 values */
    static const seq_t first[12] = {
        (seq_t)1, (seq_t)8, (seq_t)32, (seq_t)88, (seq_t)192, (seq_t)360,
        (seq_t)608, (seq_t)832, (seq_t)960, (seq_t)1040, (seq_t)1112,
        (seq_t)1212
    };

    if (n < 12) {
        return first[n];
    } else {
        /* compute 124*n - 156 using seq_t arithmetic */
        seq_t result = (seq_t)124 * (seq_t)n - (seq_t)156;
        return result;
    }
}
