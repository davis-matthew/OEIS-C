/*
 * A305762  a(0) = 24, a(n) = 2^(max(0, min(3, p - 1))) * 3^(max(0, min(1, q -
 *          1))) where n = 2^p * 3^q * 5^r * ... .
 * https://oeis.org/A305762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A305762: a(0)=24, for n>=0
   a(n) = gcd(24, n / gcd(6,n))
   The sequence is periodic with period 144.  */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* reduce n by the period */
    idx_t r = n % (idx_t)144;

    /* work in seq_t, small numbers */
    seq_t sn = (seq_t)r;

    seq_t g6 = gcd((seq_t)6, sn);
    seq_t t  = sn / g6;              /* n / gcd(6,n) */
    return gcd((seq_t)24, t);        /* final value */
}
