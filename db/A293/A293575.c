/*
 * A293575  Difference between the number of proper divisors of n and the
 *          number of squares dividing n.
 * https://oeis.org/A293575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = number_of_proper_divisors(n) -
 *            number_of_square_divisors(n) * = d(n) - 1 - sq(n) * where d(n)
 *            is the total divisor count and sq(n) counts all square divisors
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n <= 1)        /* n==1 gives the only non‑zero negative term */
        return (seq_t)-1;

    seq_t d = 1;       /* total divisor count d(n) */
    seq_t sq = 1;      /* number of square divisors sq(n) */
    idx_t m = n;
    /* Trial division up to sqrt(m) */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            d *= (seq_t)(e + 1);
            sq *= (seq_t)(e / 2 + 1);
        }
    }
    /* Remaining prime factor > 1 (exponent 1) */
    if (m > 1) {
        d *= (seq_t)2;   /* exponent 1 -> factor (1+1) */
        /* sq remains unchanged because floor(1/2)+1 = 1 */
    }

    /* a(n) = d(n) - 1 - sq(n) */
    return d - 1 - sq;
}
