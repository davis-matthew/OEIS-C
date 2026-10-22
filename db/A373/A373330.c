/*
 * A373330  a(n) is the difference between T = A000217(n^2) and the greatest
 *          square not exceeding T.
 * https://oeis.org/A373330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n^2*(n^2+1)/2 - floor_sqrt(n^2*(n^2+1)/2)^2
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 594, u64 n <= 9998,
 *            u128 n <= 9998, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t n_sq = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t T    = n_sq * (n_sq + (seq_t)1) / 2; /* triangular number of n^2 */
    seq_t s    = floor_sqrt(T);                /* greatest integer sqrt <= sqrt(T) */
    return T - s * s;                          /* difference from the next lower square */
}
