/*
 * A356217  a(n) = A022839(A000201(n)).
 * https://oeis.org/A356217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(5)*floor(n*phi)), phi=(1+sqrt(5))/2; i.e.
 *            A022839(A000201(n))
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
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
    seq_t nn = (seq_t)n;
    /* A000201(n) = floor(n*phi) = (n + isqrt(5n^2)) / 2 */
    seq_t r = isqrt_seq((seq_t)5 * nn * nn);
    seq_t m = (nn + r) / 2;
    /* A022839(m) = floor(m*sqrt(5)) = isqrt(5m^2) */
    return isqrt_seq((seq_t)5 * m * m);
}
