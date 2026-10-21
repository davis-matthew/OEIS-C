/*
 * A276042  Exponential convolution of central polygonal numbers (A000124)
 *          with themselves.
 * https://oeis.org/A276042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 2*n^3 + 19*n^2 + 42*n + 64) * 2^(n-6) with the
 *            convention that 2^(n-6) is interpreted as a divisor for n < 6,
 *            i.e. a(n) = (n^4 + 2*n^3 + 19*n^2 + 42*n + 64) >> (6-n)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute n^2, n^3, n^4 in seq_t */
    seq_t n_seq = (seq_t)n;
    seq_t n2   = n_seq * n_seq;
    seq_t n3   = n2 * n_seq;
    seq_t n4   = n3 * n_seq;

    /* polynomial part */
    seq_t poly = n4           // n^4
              + (2 * n3)      // + 2*n^3
              + (19 * n2)     // + 19*n^2
              + (42 * n_seq)  // + 42*n
              + 64;           // + 64

    if (n >= 6) {
        seq_t res = poly;
        idx_t shift = n - 6;
        while (shift--)               /* avoid UB of shifts >= width */
            res *= 2;
        return res;
    } else {
        seq_t res = poly;
        idx_t shift = 6 - n;
        while (shift--)               /* floor division by power of two */
            res >>= 1;
        return res;
    }
}
