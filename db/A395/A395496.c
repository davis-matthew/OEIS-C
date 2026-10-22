/*
 * A395496  a(n) = Sum_{d|n} gcd(d,n/d)*mu(d).
 * https://oeis.org/A395496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Sum_{d|n} gcd(d,n/d)*mu(d) Multiplicative with a(p) = 0
 *            and a(p^e) = 1-p for e ≥ 2
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* a(1)=1 */

    uint64_t m = n;                /* working copy */
    seq_t prod = 1;                /* product of (1-p) over prime powers */

    /* Handle factor 2 separately for speed */
    if (m % 2 == 0) {
        uint64_t exp = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++exp;
        }
        if (exp == 1) return 0;          /* prime power with exponent 1 -> zero */
        prod *= (seq_t)(1 - (seq_t)2);
    }

    /* Test odd divisors up to sqrt(m) */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (exp == 1) return 0;      /* exponent 1 -> zero */
            prod *= (seq_t)(1 - (seq_t)p);
        }
    }

    /* If any prime factor remains with exponent 1, result is zero */
    if (m > 1)
        return 0;

    return prod;
}
