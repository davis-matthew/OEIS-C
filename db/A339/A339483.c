/*
 * A339483  Number of regular polygons that can be drawn with vertices on a
 *          centered hexagonal grid with side length n.
 * https://oeis.org/A339483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(n+1)*n*(2*n+1)^2
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 214, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* seq_t and idx_t are defined by the build environment.  */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t to avoid overflow in intermediate
       computations when seq_t is larger than the native integer type. */
    seq_t nn = (seq_t)n;
    seq_t t1 = nn * (nn + 1);     /* n*(n+1) */
    t1 /= 2;                      /* divide by 2 first */
    seq_t t2 = 2 * nn + 1;        /* 2*n+1 */
    t1 *= t2 * t2;                /* multiply by (2*n+1)^2 */
    return t1;
}
