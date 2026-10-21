/*
 * A245552  G.f.: Sum_{n>=0} (2*n+1)*x^(n^2+n+1).
 * https://oeis.org/A245552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = √(4n−3) if √(4n−3) is an odd integer, otherwise 0
 * Range:     u8 n <= 16, u16 n <= 4001, u32 n <= 9973, u64 n <= 9973,
 *            u128 n <= 9973, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* Compute D = 4*n - 3 */
    seq_t D = (seq_t)4 * (seq_t)n - (seq_t)3;
    seq_t s = floor_sqrt(D);
    if (s * s != D) return 0;     /* D is not a perfect square   */
    if ((s & (seq_t)1) == 0) return 0; /* sqrt is even -> even index  */
    return s;                     /* value of the sequence term   */
}
