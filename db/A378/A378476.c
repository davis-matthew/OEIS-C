/*
 * A378476  The number of n-colorings of the vertices of the truncated
 *          dodecahedron up to rotation and reflection.
 * https://oeis.org/A378476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/120)*(n^60 + 15*n^32 + 16*n^30 + 20*n^20 + 24*n^12 +
 *            20*n^10 + 24*n^6)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert index to primary integer type */
    seq_t sn   = (seq_t)n;
    seq_t pow6  = big_pow(sn, (seq_t)6);
    seq_t pow10 = big_pow(sn, (seq_t)10);
    seq_t pow12 = big_pow(sn, (seq_t)12);
    seq_t pow20 = big_pow(sn, (seq_t)20);
    seq_t pow30 = big_pow(sn, (seq_t)30);
    seq_t pow32 = big_pow(sn, (seq_t)32);
    seq_t pow60 = big_pow(sn, (seq_t)60);

    seq_t numerator =
        pow60
        + (seq_t)15 * pow32
        + (seq_t)16 * pow30
        + (seq_t)20 * pow20
        + (seq_t)24 * pow12
        + (seq_t)20 * pow10
        + (seq_t)24 * pow6;

    return numerator / (seq_t)120;
}
