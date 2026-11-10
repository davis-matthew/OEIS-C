/*
 * A356088  a(n) = A001951(A022838(n)).
 * https://oeis.org/A356088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001951(A022838(n)) = floor(floor(n*sqrt(3))*sqrt(2)),
 *            computed with exact integer square roots
 * Range:     u8 n <= 6, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-10)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t m = isqrt_seq(sn * sn * 3);   /* floor(n*sqrt(3)) */
    return isqrt_seq(m * m * 2);        /* floor(m*sqrt(2)) */
}
