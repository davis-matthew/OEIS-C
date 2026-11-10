/*
 * A356182  a(n) = A022838(A001952(n)).
 * https://oeis.org/A356182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(3)*m) = isqrt(3*m^2), where m = A001952(n) =
 *            2*n + isqrt(2*n^2)
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t m = nn + nn + isqrt_seq(2 * nn * nn);
    return isqrt_seq(3 * m * m);
}
