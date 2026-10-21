/*
 * A257853  a(n) = 2*n^3 - floor(2^(1/3)*n)^3.
 * https://oeis.org/A257853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3 - floor(2^(1/3)*n)^3
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* n as seq_t for safe intermediate arithmetic */
    seq_t nn   = (seq_t)n;
    seq_t n3   = nn * nn * nn;   /* n^3 */
    seq_t twon = 2 * n3;          /* 2*n^3 */

    /* Floor of the cube root of 2*n^3, which is the same
       as floor(2^(1/3)*n).  Use the provided utility. */
    seq_t k = floor_cube_root(twon);

    return twon - k * k * k;
}
