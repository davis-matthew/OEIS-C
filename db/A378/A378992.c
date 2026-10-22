/*
 * A378992  a(n) = A011371(n) - A048881(n); The exponent of the highest power
 *          of 2 dividing the n-th factorial minus the exponent of the highest
 *          power of 2 dividing n-th Catalan number.
 * https://oeis.org/A378992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*v2(n!) - v2((2*n)!) + v2(n+1), where v2(x) = exponent
 *            of the highest power of 2 dividing x
 * Range:     u8 n <= 259, u16 n <= 9786, u32 n <= 9786, u64 n <= 9786,
 *            u128 n <= 9786, big any n
 * Helpers:   trailing_zeros_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/trailing_zeros_idx.h"

static idx_t popcount_idx(idx_t x)
{
    /* count bits set to 1 in x */
    idx_t c = 0;
    while (x)
    {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* v2(n!) = n - popcount(n) */
    seq_t A = (seq_t)n - (seq_t)popcount_idx(n);

    /* v2((2n)!) = 2n - popcount(2n) */
    seq_t B = (seq_t)(n << 1) - (seq_t)popcount_idx(n << 1);

    /* v2(n+1) = trailing zeros of n+1 */
    seq_t C = (seq_t)trailing_zeros_idx((idx_t)n + 1);

    /* a(n) = 3*A - B + C */
    return (seq_t)(3 * A - B + C);
}
