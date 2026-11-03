/*
 * A303449  Denominator of (2*n+1)/(2^(2*n+1)-1).
 * https://oeis.org/A303449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = (2^(2*n+1)-1)/gcd(2*n+1, 2^(2*n+1)-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* exponent of 2 in the denominator */
    idx_t exp = 2 * n + 1;

    /* numerator of the fraction */
    seq_t num = (seq_t)(2 * n + 1);

    /* denominator of the fraction, before reduction */
    seq_t denom = ipow_idx((seq_t)2, exp) - 1;

    /* greatest common divisor of numerator and denominator */
    seq_t g = gcd(num, denom);

    /* reduced denominator */
    return denom / g;
}
