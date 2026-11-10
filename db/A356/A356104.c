/*
 * A356104  a(n) = A000201(A022839(n)).
 * https://oeis.org/A356104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000201(A022839(n)) = floor(phi*floor(n*sqrt(5))), phi =
 *            (1+sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 58, u64 n <= 58, u128 n <= 58,
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
    seq_t N = (seq_t)n;
    /* A022839(n) = floor(sqrt(5 n^2)) */
    seq_t m = isqrt_seq(5 * N * N);
    /* A000201(m) = floor((m + sqrt(5 m^2))/2) = m + (isqrt(5 m^2) - m)/2 */
    seq_t r = isqrt_seq(5 * m * m);
    return m + (r - m) / 2;
}
