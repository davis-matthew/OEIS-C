/*
 * A377721  Numbers that are not of the form k(k+1)(2k+1)/6 + k.
 * https://oeis.org/A377721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + m if n > m(m+1)(2m+1)/6 * a(n) = n + m-1 otherwise *
 *            where m = floor((3n)^(1/3))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   icube_root_3_mul_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/icube_root_3_mul_u64.h"

seq_t a(idx_t n)
{
    /* n is at least 1 according to the OEIS offset. */
    idx_t m = icube_root_3_mul_u64(n);
    seq_t ms = (seq_t)m;
    seq_t s  = ms * (ms + 1) * (2 * ms + 1) / 6;   /* m(m+1)(2m+1)/6  */
    seq_t nn = (seq_t)n;
    return (nn > s) ? nn + m : nn + m - 1;
}
