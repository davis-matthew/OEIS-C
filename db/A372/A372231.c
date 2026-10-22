/*
 * A372231  Fixed points of A372341.
 * https://oeis.org/A372231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+3)/2 - floor(n*phi) where phi = (1+sqrt(5))/2 Using
 *            floor(n*phi) = (n + floor(sqrt(5*n*n)))/2 Offset: 1 (a(1) = 1)
 * Range:     u8 n <= 3, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Promote n to seq_t for arithmetic. */
    seq_t nn = n;

    /* Triangular-like term: n*(n+3)/2. */
    seq_t tri = nn * (nn + 3) / 2;

    /* Compute floor(sqrt(5*n*n)).  Use seq_t to make it work for arbitrary precision. */
    seq_t sq = nn * nn;      /* n*n */
    sq *= 5;                 /* 5*n*n */
    seq_t root = floor_sqrt(sq);

    /* floor(n*phi) = (n + floor(sqrt(5*n*n))) / 2 */
    seq_t floor_phi = (nn + root) / 2;

    return tri - floor_phi;
}
