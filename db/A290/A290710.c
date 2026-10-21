/*
 * A290710  Number of irredundant sets in the n-crown graph.
 * https://oeis.org/A290710
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+1) + n*(n^3 - 2*n^2 + 3*n + 2)/4 - 1 for n > 3
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 3)
        return (seq_t)24;

    seq_t n_t = (seq_t)n;
    seq_t n2  = n_t * n_t;
    seq_t n3  = n2 * n_t;

    seq_t inside = n3 - ( (seq_t)2 * n2 ) + ( (seq_t)3 * n_t ) + (seq_t)2;
    seq_t prod   = n_t * inside;
    seq_t div4   = prod / (seq_t)4;

    seq_t pow2   = big_pow((seq_t)2, (seq_t)(n + 1));

    return pow2 + div4 - (seq_t)1;
}
