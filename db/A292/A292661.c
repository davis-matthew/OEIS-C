/*
 * A292661  Rank of n*sqrt(3) when all the numbers h*sqrt(2), j*sqrt(3),
 *          k*sqrt(5), for h>=1, j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊ n*√(3/2) ⌋ + ⌊ n*√(3/5) ⌋ Computed without floating
 *            point by using integer inequalities: floor(n*√(3/2)) = floor(
 *            √(3*n²/2) ) floor(n*√(3/5)) = floor( √(3*n²/5) )
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n = 0, but safe to return 0 */

    seq_t nn = (seq_t)n;
    seq_t t = nn * nn;     /* n², fits in seq_t for the expected n range */
    t = t * (seq_t)3;      /* 3*n² */

    seq_t k1 = sqrt_floor(t / (seq_t)2);   /* floor(√(3*n²/2)) */
    seq_t k2 = sqrt_floor(t / (seq_t)5);   /* floor(√(3*n²/5)) */

    return nn + k1 + k2;
}
