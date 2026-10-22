/*
 * A346461  a(n) = 2^A042965(n+1).
 * https://oeis.org/A346461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(floor((4*n+1)/3))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute a(n) = 2^((4*n+1)/3) */
    idx_t exp = (4 * n + 1) / 3;
    return pow2(exp);
}
