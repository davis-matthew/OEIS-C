/*
 * A289793  Number of 5-cycles in the n-tetrahedral graph.
 * https://oeis.org/A289793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*binomial(n,5)*(-78 + 21*n + n^2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 5u)
        return (seq_t)0;
    seq_t bin = binomial(n, 5);
    seq_t nn = (seq_t)n;
    seq_t term = nn * nn + (seq_t)21 * nn - (seq_t)78;
    return (seq_t)6 * bin * term;
}
