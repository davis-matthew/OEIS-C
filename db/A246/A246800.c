/*
 * A246800  Even-indexed terms of A247984, a sequence motivated by generalized
 *          quadrangles.
 * https://oeis.org/A246800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(2n) - (-1)^n * binomial(2n, n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0,
       although the offset is 1, this is harmless. */
    if (n < 1) return 0;

    seq_t p = pow2(2 * n);          /* 2^(2n) */
    seq_t b = binomial(2 * n, n);   /* binomial(2n, n) */

    /* (-1)^n is 1 when n is even, -1 when n is odd.
       Thus for even n we subtract, for odd n we add. */
    if ((n & 1) == 0)
        return p - b;   /* even n: 2^(2n) - binomial(2n,n) */
    else
        return p + b;   /* odd n:  2^(2n) + binomial(2n,n) */
}
