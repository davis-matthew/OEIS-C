/*
 * A268967  Number of n X 4 0..2 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A268967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16 * 3^(2*n-3) * (8*n+3) for n >= 2; a(1) = 60
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* provides big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)60;
    }

    /* exponent 2*n-3 fits in idx_t for n up to at least 10000 */
    idx_t exp = (idx_t)2 * n - (idx_t)3;

    /* compute 3^(2*n-3) */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)exp);

    /* term = 8*n + 3 */
    seq_t term = (seq_t)((idx_t)8 * n + (idx_t)3);

    /* a(n) = 16 * pow3 * term */
    seq_t res = pow3;
    res *= term;
    res *= (seq_t)16;

    return res;
}
