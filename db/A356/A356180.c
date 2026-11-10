/*
 * A356180  a(n) = A022838(A001951(n)).
 * https://oeis.org/A356180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A022838(A001951(n)) = floor(floor(n*sqrt(2))*sqrt(3)) =
 *            isqrt(3*isqrt(2*n^2)^2)
 * Range:     u8 n <= 7, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-10)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t nn = (seq_t)n;
    seq_t m = isqrt_seq(nn * nn * 2);      /* A001951(n) = floor(n*sqrt(2)) */
    return isqrt_seq(m * m * 3);           /* A022838(m) = floor(m*sqrt(3)) */
}
