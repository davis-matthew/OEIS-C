/*
 * A333471  a(n) = 2 * mu(n) + Sum_{d|n, d > 1} mu(n/d) * (prime(d) -
 *          prime(d-1)).
 * https://oeis.org/A333471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * A054541(d) where A054541(1)=2 and for
 *            d>1, A054541(d)=prime(d)-prime(d-1)
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   get_prime, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined for n=0, return 0 for safety */

    seq_t result = 0;
    idx_t i, d1, d2;

    for (i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            d1 = i;
            d2 = n / i;

            /* term for divisor d1 */
            seq_t mu1 = mobius(n / d1);  /* n/d1 == d2 */
            seq_t term1;
            if (d1 == 1) {
                term1 = mu1 * (seq_t)2;
            } else {
                seq_t p_d1 = (seq_t)get_prime(d1);
                seq_t p_d1m1 = (seq_t)get_prime(d1 - 1);
                term1 = mu1 * (p_d1 - p_d1m1);
            }
            result += term1;

            /* term for divisor d2, if distinct */
            if (d1 != d2) {
                seq_t mu2 = mobius(n / d2);  /* n/d2 == d1 */
                seq_t term2;
                if (d2 == 1) {
                    term2 = mu2 * (seq_t)2;
                } else {
                    seq_t p_d2 = (seq_t)get_prime(d2);
                    seq_t p_d2m1 = (seq_t)get_prime(d2 - 1);
                    term2 = mu2 * (p_d2 - p_d2m1);
                }
                result += term2;
            }
        }
    }
    return result;
}
