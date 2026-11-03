/*
 * A394911  a(n) = denominator( (2/3)^n*(1/8 - (1/24)*(1/9)^n) ).
 * https://oeis.org/A394911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(0)=12, a(1)=162, a(n)=27*a(n-1) for n>2 (equivalently
 *            a(n)=3^(3*n+1) for n>1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 12;
    if (n == 1) return 162;
    /* For n >= 2: a(n) = 3^(3*n+1) */
    return ipow((seq_t)3, (seq_t)(3 * n + 1));
}
