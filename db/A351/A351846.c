/*
 * A351846  Irregular triangle read by rows: T(n,k), n >= 0, k >= 0, in which
 *          n appears 4*n + 1 times in row n.
 * https://oeis.org/A351846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = floor((sqrt(8*n + 1) + 1)/4)
 * Range:     u8 n <= 7, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t val = (seq_t)n;
    /* Compute 8*n + 1 in seq_t, safe for 64/128 bits */
    seq_t x = val * 8 + 1;
    /* Integer square root of x */
    seq_t s = floor_sqrt(x);
    /* floor((s + 1)/4) */
    return (s + 1) / 4;
}
