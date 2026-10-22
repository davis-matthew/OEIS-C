/*
 * A308580  a(n) = 3*2^n + n^2 - n.
 * https://oeis.org/A308580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n + n^2 - n
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t pow2n = pow2(n);              /* 2^n in seq_t */

    /* 3 * 2^n */
    seq_t part1 = pow2n * 3;

    /* n^2 - n = n * (n-1) */
    seq_t quad = 0;
    if (n != 0) {
        seq_t n_seq = (seq_t)n;
        quad = n_seq * (n_seq - 1);
    }

    return part1 + quad;
}
