/*
 * A387599  Number of ways to tile a strip of length n with squares, dominoes
 *          and trominoes, using an equal number of dominoes and trominoes.
 * https://oeis.org/A387599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..floor(n/5)} binomial(n-3*i, 2*i) *
 *            binomial(2*i,i)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t limit = n / 5;
    for (idx_t i = 0; i <= limit; ++i)
    {
        seq_t term1 = binomial(n - 3 * i, 2 * i);
        seq_t term2 = binomial(2 * i, i);
        result += term1 * term2;
    }
    return result;
}
