/*
 * A248152  a(n) = 48 * 4^n * (2*n-1)!! * (2*n+3)!! / ((n+2)! * (n+4)!).
 * https://oeis.org/A248152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A248152:
   a(0) = 3
   a(n) = 4 * (2n+3) * (2n-1) * a(n-1) / ((n+4) * (n+2))   for n >= 1
   The division always yields an integer.  */
#include "../utils/gcd.h"

extern seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;

    seq_t a_val = (seq_t)3;                     /* a(0)          */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* numerator and denominator of the factor */
        seq_t num = (seq_t)4;
        num *= (seq_t)(2 * i + 3);              /* * (2i+3)      */
        num *= (seq_t)(2 * i - 1);              /* * (2i-1)      */

        seq_t den = (seq_t)(i + 4) * (seq_t)(i + 2);   /* (i+4)(i+2) */

        /* Reduce by cancelling common divisors with a_val
           first, then with the numerator.  This keeps all
           intermediate values as small as possible. */
        seq_t g = gcd(a_val, den);
        a_val /= g;
        den /= g;

        g = gcd(num, den);
        num /= g;
        den /= g;                               /* den is now 1 */

        a_val *= num;                          /* exact multiplication */
    }
    return a_val;
}
