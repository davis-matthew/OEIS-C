/*
 * A351632  Number of copies of the star graph S(2,1,1,1) contained in the
 *          n-dimensional hypercube graph.
 * https://oeis.org/A351632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * binomial(n,3) * (n-1)*(n-3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 15, u64 n <= 40, u128 n <= 97,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    seq_t res = binomial(n, 3);                 /* C(n,3) */
    seq_t t   = (seq_t)(n - 1) * (seq_t)(n - 3); /* (n-1)(n-3) */
    res *= t;                                   /* C(n,3)*(n-1)(n-3) */
    res *= pow2(n);                              /* *2^n */
    return res;
}
