/*
 * A356186  Number of labeled trees on [2n] with a bicentroid.
 * https://oeis.org/A356186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2n,n)*n^(2n-2)/2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t two_n = n + n;
    seq_t bin = binomial(two_n, n);

    seq_t exp = (seq_t)n * 2;
    exp -= 2;                 /* exp = 2n-2 */

    seq_t powN = big_pow((seq_t)n, exp);
    seq_t res   = bin * powN;
    res = res / 2;            /* divide by 2 */

    return res;
}
