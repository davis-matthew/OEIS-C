/*
 * A296196  Harary index of the n X n queen graph.
 * https://oeis.org/A296196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n - 1)*(3*n^2 + 13*n - 2)/12
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    seq_t nn = (seq_t)n;

    /* Compute n*(n-1) and reduce it by the GCD with 12 to avoid overflow */
    seq_t prod = nn * (nn - 1);
    seq_t d    = gcd(prod, (seq_t)12);
    prod      /= d;
    seq_t rem  = (seq_t)12 / d;

    /* Compute the remaining factor: 3*n^2 + 13*n - 2 */
    seq_t nn_sq = nn * nn;
    seq_t term  = (seq_t)3 * nn_sq + (seq_t)13 * nn - (seq_t)2;

    /* Final multiplication and division by the remaining part of 12 */
    prod *= term;
    prod /= rem;

    return prod;
}
