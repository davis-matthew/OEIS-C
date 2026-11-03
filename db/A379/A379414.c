/*
 * A379414  a(n) = n + floor(n*s/r) + floor(n*t/r), where r = 3^(1/4), s =
 *          3^(1/2), t = 3^(3/4).
 * https://oeis.org/A379414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*3^(1/4)) + floor(n*sqrt(3))
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    /* floor(n * sqrt(3)) = floor( sqrt(n^2 * 3) ) */
    seq_t n2 = sn * sn;
    seq_t floor_n_sqrt3 = isqrt_seq(n2 * 3);

    /* floor(n * 3^(1/4)) = floor( (n^4 * 3)^(1/4) ) */
    seq_t n4 = n2 * n2;              /* n^4 */
    seq_t tmp = isqrt_seq(n4 * 3);   /* floor( sqrt(n^4 * 3) ) */
    seq_t floor_n_r = isqrt_seq(tmp);/* floor( sqrt( floor( sqrt(n^4 * 3) ) ) ) */

    return sn + floor_n_r + floor_n_sqrt3;
}
