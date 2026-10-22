/*
 * A378477  The number of n-colorings of the vertices of the truncated
 *          icosidodecahedron up to rotation and reflection.
 * https://oeis.org/A378477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/120*(n^120 + 31*n^60 + 20*n^40 + 24*n^24 + 20*n^20 +
 *            24*n^12)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* compute each power once */
    seq_t p12  = big_pow(N, 12);
    seq_t p20  = big_pow(N, 20);
    seq_t p24  = big_pow(N, 24);
    seq_t p40  = big_pow(N, 40);
    seq_t p60  = big_pow(N, 60);
    seq_t p120 = big_pow(N, 120);

    /* accumulate weighted sum */
    seq_t res = p120;
    res += (seq_t)31 * p60;
    res += (seq_t)20 * p40;
    res += (seq_t)24 * p24;
    res += (seq_t)20 * p20;
    res += (seq_t)24 * p12;

    /* divide by group order 120 */
    res /= 120;

    return res;
}
