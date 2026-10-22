/*
 * A353545  a(n) is the numerator of Sum_{k=1..n} 1 / (k*k!).
 * https://oeis.org/A353545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the numerator of Sum_{k=1..n} 1 / (k*k!)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t fact = 1;          /* k!   (kept in arbitrary precision) */
    seq_t num  = 0;          /* numerator of the reduced sum */
    seq_t den  = 1;          /* denominator of the reduced sum */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update factorial: fact = k! */
        fact = fact * (seq_t)k;

        /* dk = k * k! (the denominator of the new term) */
        seq_t dk = fact * (seq_t)k;

        /* Reduce dk with current denominator to keep intermediate
           values small (gcd of den and dk).  Let
               g  = gcd(den, dk)
               den' = den   / g
               dk'  = dk    / g
           The least common multiple of den and dk is den * dk'  (or
           equivalently den' * dk).  Using this we can add the fractions
           without ever forming den * dk. */
        seq_t g   = gcd(den, dk);
        seq_t denp = den / g;       /* den'  */
        seq_t dkp  = dk  / g;       /* dk'   */

        /* New fraction (num/den) + 1/dk  →  (num*dkp + denp) / (den * dkp) */
        seq_t new_num  = num * dkp + denp;
        seq_t new_den  = den * dkp;          /* den * dk'  */

        /* Reduce the result to keep it fully reduced */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;    /* numerator of the reduced sum */
}
