/*
 * A306545  Number of permutations p of [2n+2] such that min_{j=1..2n+2}
 *          |p(j)-j| = n.
 * https://oeis.org/A306545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 25*4^(n-2) + 4*3^(n-2) - 1 for n >= 2 a(0) = 1, a(1) = 8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)8;
    idx_t exp = n - 2;
    seq_t term4 = seq_pow((seq_t)4, exp);
    seq_t term3 = seq_pow((seq_t)3, exp);
    seq_t res = (seq_t)25 * term4;
    res += (seq_t)4 * term3;
    res -= (seq_t)1;
    return res;
}
