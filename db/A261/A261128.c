/*
 * A261128  Cyclotomic polynomial value Phi(7,n!).
 * https://oeis.org/A261128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Φ(7, n!) = (n!)^6 + (n!)^5 + … + (n!) + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   factorial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t fact = factorial(n);             /* n! */
    seq_t sum  = (seq_t)1;                 /* start with 1   */
    seq_t pow  = fact;                     /* current power f^k */

    for (int k = 1; k <= 6; ++k)
    {
        sum = sum + pow;                   /* add f^k      */
        if (k < 6)                         /* compute next power */
            pow = seq_mul(pow, fact);
    }

    return sum;
}
