/*
 * A342761  Fold a square sheet of paper alternately vertically to the left
 *          and horizontally downwards; after each fold, draw a line along
 *          each inward crease; after n folds, the resulting graph has a(n)
 *          edges.
 * https://oeis.org/A342761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 4 a(2*t) = 2^(2*t) + 3*2^(t-1) + 3 for t >= 1 a(2*t+1) =
 *            2^(2*t+1) + 2^(t+1) + 3 for t >= 0 (t derived from n as
 *            floor(n/2) or (n-1)/2 as appropriate)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)4u;

    if ((n & 1u) == 0u)          /* even n */
    {
        idx_t t = n / 2u;        /* t >= 1 for n >= 2 */
        seq_t term1 = pow2(2 * t);          /* 2^(2*t)   */
        seq_t term2 = pow2(t - 1u);         /* 2^(t-1)   */
        term2 = term2 * (seq_t)3u;          /* 3*2^(t-1) */
        return term1 + term2 + (seq_t)3u;
    }
    else                          /* odd n */
    {
        idx_t t = (n - 1u) / 2u;                   /* t >= 0      */
        seq_t term1 = pow2(2 * t + 1u);            /* 2^(2*t+1)   */
        seq_t term2 = pow2(t + 1u);                /* 2^(t+1)     */
        return term1 + term2 + (seq_t)3u;
    }
}
