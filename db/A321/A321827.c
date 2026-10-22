/*
 * A321827  a(n) = Sum_{d|n, d==1 (mod 4)} d^11 - Sum_{d|n, d==3 (mod 4)}
 *          d^11.
 * https://oeis.org/A321827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Sum_{d|n, d==1 (mod 4)} d^11 - Sum_{d|n, d==3 (mod 4)}
 *            d^11
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 8, i64 n <= 52, i128 n <= 2990,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

/* Compute the multiplicative factor contributed by an odd prime power p^exp.
   For p ≡ 1 (mod 4): 1 + p^11 + p^(22) + … + p^(11*exp)
   For p ≡ 3 (mod 4): 1 - p^11 + p^(22) – …  (alternating signs) */
static seq_t factor_from_prime(uint64_t p, int exp)
{
    seq_t factor = 1;          /* k = 0 term */
    seq_t cur    = 1;          /* p^(11*k) */
    seq_t p11    = pow_seq((seq_t)p, 11);

    if (p % 4 == 1) {
        for (int k = 0; k < exp; ++k) {
            cur *= p11;
            factor += cur;
        }
    } else {                    /* p % 4 == 3 */
        int sign = 1;          /* sign for cur term beyond k = 0 */
        for (int k = 0; k < exp; ++k) {
            cur *= p11;
            sign = -sign;
            factor += sign * cur;
        }
    }
    return factor;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* Remove all factors of 2 – their contribution is 1 */
    while ((m & 1) == 0) m >>= 1;
    if (m == 1) return result;

    /* Factor the odd part of n */
    for (uint64_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {
            int exp = 0;
            while (m % d == 0) {
                m /= d;
                ++exp;
            }
            result *= factor_from_prime(d, exp);
        }
    }

    /* Any remaining prime factor greater than sqrt(m) */
    if (m > 1)
        result *= factor_from_prime(m, 1);

    return result;
}
