/*
 * A340615  a(n) = k/2 if k is even, otherwise (3k+1)/2, where k = n +
 *          floor((n+1)/5).
 * https://oeis.org/A340615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k/2 if k is even, otherwise (3k+1)/2, where k = n +
 *            floor((n+1)/5)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute k = n + floor((n+1)/5) */
    seq_t k = (seq_t)n + (((seq_t)n + 1) / (seq_t)5);

    /* apply the piecewise definition */
    if ((k % (seq_t)2) == (seq_t)0)
        return k / (seq_t)2;
    else
        return (k * (seq_t)3 + (seq_t)1) / (seq_t)2;
}
