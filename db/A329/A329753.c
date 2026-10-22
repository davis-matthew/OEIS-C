/*
 * A329753  Doubly square pyramidal numbers.
 * https://oeis.org/A329753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000330(A000330(n))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   square_pyramid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/square_pyramid.h"
/* A000330(n) = n(n+1)(2n+1)/6, the square pyramidal number */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t t = square_pyramid(n_seq);
    return square_pyramid(t);
}
