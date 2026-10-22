/*
 * A394927  a(n) is the minimal sum of squares over partitions of n into
 *          distinct parts.
 * https://oeis.org/A394927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{i=1}^{k} i^2 + r\,(2k+2-r), where * k =
 *            ⌊(sqrt(8n+1)-1)/2⌋ and r = n - k(k+1)/2 * This gives the minimal
 *            sum of squares of a partition of n into distinct parts
 * Range:     u8 n <= 7, u16 n <= 1676, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* Compute k = floor((sqrt(8N+1)-1)/2) */
    seq_t t = floor_sqrt(N * (seq_t)8 + (seq_t)1);   /* floor sqrt(8n+1) */
    seq_t k = (t - (seq_t)1) >> 1;                   /* integer division by 2 */

    /* Triangular number T_k = k(k+1)/2 */
    seq_t T = k * (k + (seq_t)1) / (seq_t)2;

    /* r = N - T_k */
    seq_t r = N - T;

    /* Sum of squares S_k = k(k+1)(2k+1)/6 */
    seq_t S = k * (k + (seq_t)1) * ( (seq_t)2 * k + (seq_t)1 ) / (seq_t)6;

    /* Add the extra contributions from increasing the largest r parts by 1 */
    seq_t add = r * ( ( (seq_t)2 * k + (seq_t)2 ) - r );

    return S + add;
}
