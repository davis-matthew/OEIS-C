/*
 * A372835  a(n) is the denominator of Sum_{d|n, d < sqrt(n)} 1/d.
 * https://oeis.org/A372835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sum_{d|n, d<sqrt(n)} 1/d
 * Range:     u8 n <= 59, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sum starts from 0/1.  We repeatedly add 1/d for every divisor d<n
       with d < sqrt(n) and reduce after each addition to keep the numbers
       small.  The final denominator returned is the denominator of the
       simplified rational. */
    if (n == 0) return 0;  /* Offset of OEIS is 1; n>=1 in practice */

    seq_t num = 0;          /* current numerator   */
    seq_t den = 1;          /* current denominator */

    /* iterate through all integers i satisfying i*i < n; if i divides n,
       add the term 1/i.  This guarantees that i < sqrt(n).  Note that
       divisors greater than sqrt(n) are not considered, as required. */
    for (idx_t i = 1; (__int128)i * i < n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d = i;           /* small divisor */

            seq_t temp_num = num * (seq_t)d + den;
            seq_t temp_den = den * (seq_t)d;

            seq_t g = gcd(temp_num, temp_den);
            num = temp_num / g;
            den = temp_den / g;
        }
    }

    return den;
}
