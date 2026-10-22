/*
 * A348898  a(n) = 15120*(2*n)!/(n!*(n + 5)!); super ballot numbers, row 4 of
 *          A135573.
 * https://oeis.org/A348898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 256*C(n) - 256*C(n+1) + 96*C(n+2) - 16*C(n+3) + C(n+4)
 *            where C(k) = A000108(k) = Catalan number. The sequence satisfies
 *            a(n) = (n+5)\u00D7a(n+1) = 2(2n−1)a(n) with a(0)=126
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Compute the required Catalan numbers */
    seq_t c0 = catalan(n);
    seq_t c1 = catalan(n + 1);
    seq_t c2 = catalan(n + 2);
    seq_t c3 = catalan(n + 3);
    seq_t c4 = catalan(n + 4);

    /* Combine them with the given integer coefficients */
    seq_t res = 256 * c0;
    res -= 256 * c1;
    res += 96 * c2;
    res -= 16 * c3;
    res += c4;

    return res;
}
