/*
 * A362964  Number of semimagic quad squares that can be formed using cards
 *          from Quads-2^n deck.
 * https://oeis.org/A362964
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(2^n-1)*(2^n-2)*(2^n-4)*(2^n-8)* * (112 +
 *            2823*(2^n-16) + 2531*(2^n-16)*(2^n-32) * +
 *            159*(2^n-16)*(2^n-32)*(2^n-64) * +
 *            (2^n-16)*(2^n-32)*(2^n-64)*(2^n-128))
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    seq_t t = pow2(n);                 /* t = 2^n */

    /* First product: t*(t-1)*(t-2)*(t-4)*(t-8) */
    seq_t prod = t;
    prod *= (t - 1);
    prod *= (t - 2);
    prod *= (t - 4);
    prod *= (t - 8);

    /* Inner polynomial components */
    seq_t x = (t - 16);
    seq_t y = (t - 32);
    seq_t z = (t - 64);
    seq_t w = (t - 128);

    seq_t B = x * y;         /* x*y */
    seq_t C = B * z;         /* x*y*z */
    seq_t D = C * w;         /* x*y*z*w */

    seq_t inner = 112;
    inner += (seq_t)2823 * x;
    inner += (seq_t)2531 * B;
    inner += (seq_t)159   * C;
    inner += D;

    prod *= inner;

    return prod;
}
