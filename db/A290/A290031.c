/*
 * A290031  Number of 6-cycles in the n-hypercube graph.
 * https://oeis.org/A290031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1)*binomial(n,3)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;
    seq_t b = binomial(n, (idx_t)3);
    seq_t p = pow2((idx_t)(n + 1));
    return b * p;
}
