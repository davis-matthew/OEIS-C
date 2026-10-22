/*
 * A387646  Characteristic function of A000537.
 * https://oeis.org/A387646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = ((⌊(4*n)^(1/4)⌋*(⌊(4*n)^(1/4)⌋+1)/2)^2) else 0
 * Range:     u8 n <= 15, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute 4*n in seq_t */
    seq_t m = (seq_t)n * (seq_t)4;

    /* first square root: floor(sqrt(4*n)) */
    seq_t s1 = floor_sqrt(m);

    /* second square root: k = floor((4*n)^(1/4)) */
    seq_t k = floor_sqrt(s1);

    /* triangular number t = k*(k+1)/2 */
    seq_t t = k * (k + (seq_t)1);
    t /= (seq_t)2;

    /* sum of cubes = t^2 */
    seq_t sum = t * t;

    return (sum == (seq_t)n) ? (seq_t)1 : (seq_t)0;
}
