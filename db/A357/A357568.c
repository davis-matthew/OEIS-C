/*
 * A357568  a(n) = 9*binomial(2*n,n)^2 - 8*binomial(3*n,n).
 * https://oeis.org/A357568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*binomial(2*n,n)^2 - 8*binomial(3*n,n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Compute binom(2n, n) and binom(3n, n) */
    seq_t bin2 = binom(2 * n, n);
    seq_t bin3 = binom(3 * n, n);

    /* 9 * bin2^2 */
    seq_t res = seq_mul(bin2, bin2);
    res = seq_mul(res, (seq_t)9);

    /* Subtract 8 * bin3 */
    seq_t temp = seq_mul(bin3, (seq_t)8);
    res = res - temp;

    return res;
}
