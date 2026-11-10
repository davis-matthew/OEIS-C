/*
 * A356181  a(n) = A054406(A001951(n)).
 * https://oeis.org/A356181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A054406(A001951(n)) = floor(m*(3+sqrt(3))/2) with m =
 *            floor(n*sqrt(2))
 * Range:     u8 n <= 7, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-10)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t m = isqrt_seq(2 * ns * ns);      /* floor(n*sqrt(2)) */
    seq_t s = isqrt_seq(3 * m * m);        /* floor(m*sqrt(3)) */
    seq_t k = 3 * m + s;
    return k / 2;
}
