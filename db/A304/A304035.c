/*
 * A304035  a(n) is the number of lattice points inside a square bounded by
 *          the lines x=-n/sqrt(2), x=n/sqrt(2), y=-n/sqrt(2), y=n/sqrt(2).
 * https://oeis.org/A304035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*floor_sqrt(n*n/2)+1)^2
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts the lattice points (x,y) with |x|<=n/√2 and |y|<=n/√2.
   The integer arithmetic uses floor_sqrt defined in ../utils/floor_sqrt.h. */
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n>=1 per OEIS offset */
    /* Compute m = floor(n / sqrt(2)) without using floating point. */
    /* m^2 <= n^2/2 < (m+1)^2  therefore m = floor_sqrt( floor(n^2/2) ). */
    seq_t nn  = (seq_t)n * (seq_t)n;          /* n^2  (fits in seq_t) */
    seq_t t   = nn / 2;                      /* floor(n^2/2)   */
    seq_t m   = floor_sqrt(t);               /* floor(sqrt(n^2/2)) */
    seq_t side = (m << 1) + 1;               /* 2*m + 1  */
    return side * side;                      /* (2*m+1)^2 */
}
