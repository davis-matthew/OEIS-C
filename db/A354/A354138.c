/*
 * A354138  a(n) is the numerator of Sum_{k=0..n} (-1)^k / (2*k)!.
 * https://oeis.org/A354138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of \sum_{k=0..n} (-1)^k / (2*k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t a_num = 0;          /* current numerator   */
    seq_t a_den = 1;          /* current denominator */
    seq_t fact = 1;           /* (2k)! for current k */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t denom = fact;          /* (2k)!  */

        /* Reduce the addition with current denominator */
        seq_t g = gcd(a_den, denom);
        seq_t mult1 = denom / g;     /* d/g    */
        seq_t mult2 = a_den / g;     /* b/g    */

        seq_t new_num;
        if ((k & 1) == 0) {
            /* (+) term: a_num * mult1 + mult2 */
            new_num = a_num * mult1 + mult2;
        } else {
            /* (-) term: a_num * mult1 - mult2 */
            seq_t temp = a_num * mult1;
            new_num = temp - mult2;
        }

        seq_t new_den = a_den * mult1;

        /* Reduce the new fraction */
        seq_t r = gcd(new_num, new_den);
        a_num = new_num / r;
        a_den = new_den / r;

        /* Prepare factorial for next iteration */
        if (k < n) {
            fact *= (seq_t)(2 * k + 1);
            fact *= (seq_t)(2 * k + 2);
        }
    }

    return a_num;
}
