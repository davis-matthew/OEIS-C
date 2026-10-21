/*
 * A299287  Coordination sequence for "tcd" 3D uniform tiling.
 * https://oeis.org/A299287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 1 * a(n) = (31*n^2 + 8)/4 if n > 0 is even * a(n) =
 *            (31*n^2 + 9)/4 if n > 0 is odd * This expression is equivalent
 *            to the recurrence * a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) for n >
 *            4
 * Range:     u8 n <= 2, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)1;
    }

    /* Convert n to seq_t so all arithmetic is performed in the target type */
    seq_t m = (seq_t)n;

    /* Compute 31*n^2 + (8 or 9) */
    seq_t val = m * m;   /* n^2 */
    val *= (seq_t)31;    /* 31*n^2 */
    val += ((n & 1u) == 0u) ? (seq_t)8 : (seq_t)9; /* add 8 if even, 9 if odd */

    /* The numerator is always divisible by 4 */
    val /= (seq_t)4;

    return val;
}
