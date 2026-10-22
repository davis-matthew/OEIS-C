/*
 * A302203  a(n) = floor(sin(n)) + 1.
 * https://oeis.org/A302203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sin(n)) + 1
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Compute sin as a double; the terms are 0 or 1 depending on the sign. */
    double s = sin((double)n);
    return (s >= 0.0) ? 1 : 0;
}
