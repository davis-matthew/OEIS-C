/*
 * A373163  a(n) = numerator of Sum_{k>=0} cos(2k*Pi/3)/n^k.
 * https://oeis.org/A373163
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reduced numerator of the series Σcos(2kπ/3)/n^k
   S(n) = [n*(2n+1)] / [2*(n^2+n+1)]  ; a(n) = numerator of S(n) after simplification */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* the definition is valid for n >= 2 */
    seq_t nn = (seq_t)n;
    seq_t num = nn * (((seq_t)2 * nn) + (seq_t)1);       /* n*(2n+1) */
    seq_t den = (seq_t)2 * (nn * nn + nn + (seq_t)1);     /* 2*(n^2+n+1) */
    seq_t g   = gcd(num, den);                           /* greatest common divisor */
    return num / g;                                      /* reduced numerator */
}
