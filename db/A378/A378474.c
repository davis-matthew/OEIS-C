/*
 * A378474  The number of n-colorings of the vertices of the truncated
 *          cuboctahedron up to rotation and reflection.
 * https://oeis.org/A378474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/48)*(n^48 + 19*n^24 + 8*n^16 + 12*n^12 + 8*n^8)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t fn = (seq_t)n;
    seq_t p48 = big_pow(fn, (seq_t)48);
    seq_t p24 = big_pow(fn, (seq_t)24);
    seq_t p16 = big_pow(fn, (seq_t)16);
    seq_t p12 = big_pow(fn, (seq_t)12);
    seq_t p8  = big_pow(fn, (seq_t)8);

    seq_t total = p48
                + (seq_t)19 * p24
                + (seq_t)8  * p16
                + (seq_t)12 * p12
                + (seq_t)8  * p8;

    return total / (seq_t)48;
}
