/*
 * A248231  Least k such that zeta(5) - Sum_{h = 1..k} 1/h^5 < 1/n^4.
 * https://oeis.org/A248231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( sqrt(n^2/2 - 1) + 1/2 )
 * Range:     u8 n <= 5, u16 n <= 68, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the algebraic transformation: a(n) = ( floor_sqrt(2*n*n - 4) + 1 ) / 2  for n > 1, 
   and a(1) = 1. This follows from floor( sqrt(n^2/2 - 1) + 1/2 ) = 
   floor( (sqrt(2*n*n - 4) + 1) / 2 ).  */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* handle offset */
    if (n == 1) return 1;

    /* compute t = 2*n*n - 4 safely as seq_t */
    seq_t nn = (seq_t)n;
    seq_t t = 2 * nn * nn - (seq_t)4;

    /* integer square root of t */
    seq_t s = floor_sqrt(t);

    /* result = (s + 1) / 2 */
    return (s + (seq_t)1) / (seq_t)2;
}
