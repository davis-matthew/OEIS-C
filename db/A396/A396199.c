/*
 * A396199  a(n) = (n + 1)*(2^(n + 1) - binomial(n, floor(n / 2))), row sums
 *          of A396200.
 * https://oeis.org/A396199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 1) * (2^(n + 1) - binomial(n, floor(n / 2)))
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t k = n / 2;                     /* floor(n/2) */
    seq_t twop = pow2(n + 1);            /* 2^(n+1) */
    seq_t bin = binomial(n, k);          /* C(n, floor(n/2)) */
    seq_t diff = twop - bin;             /* 2^(n+1) - C(n, floor(n/2)) */
    return (seq_t)(n + 1) * diff;       /* (n+1) * diff */
}
