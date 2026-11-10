/*
 * A343041  a(0) = 0 and for any n > 0, a(n) = A343040(a(n-1), n).
 * https://oeis.org/A343041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{p>=1} min(floor(n/p!), p) * p! (digitwise max of
 *            factorial-base digits of 0..n; a(0)=0)
 * Range:     u8 n <= 239, u16 n <= 5039, u32 n <= 5039, u64 n <= 5039,
 *            u128 n <= 5039, big any n
 * Validated: standard, pass, 83 terms through n = 5005 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t fact = 1;                 /* p! */
    for (idx_t p = 1; fact <= n; ++p) {
        idx_t q = n / fact;
        idx_t d = q < p ? q : p;
        result += (seq_t)d * (seq_t)fact;
        if (fact > n / (p + 1)) break;   /* next factorial would exceed n */
        fact *= (p + 1);
    }
    return result;
}
