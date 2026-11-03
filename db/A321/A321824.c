/*
 * A321824  a(n) = Sum_{d|n, d==1 mod 4} d^8 - Sum_{d|n, d==3 mod 4} d^8.
 * https://oeis.org/A321824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Sum_{d|n, d==1 mod 4} d^8 - Sum_{d|n, d==3 mod 4} d^8
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 14, i64 n <= 234,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 42 terms through n = 9999 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper that computes a(p^e) where p is an odd prime or 2 */
static seq_t prime_power_term(idx_t p, idx_t e)
{
    if (p == 2)          /* a(2^e) = 1 by definition */
        return 1;

    /* compute p^8 */
    seq_t p8 = 1;
    seq_t pp = (seq_t)p;
    for (int i = 0; i < 8; ++i)
        p8 *= pp;

    /* geometric series with sign depending on p mod 4 */
    seq_t sum = 1;       /* k = 0 term */
    seq_t term = 1;      /* p8^k */

    if (p % 4 == 1) {    /* all divisors give + sign */
        for (idx_t k = 1; k <= e; ++k) {
            term *= p8;
            sum += term;
        }
    } else {             /* p % 4 == 3, signs alternate */
        bool neg = true; /* k=1 term is negative */
        for (idx_t k = 1; k <= e; ++k) {
            term *= p8;
            if (neg) sum -= term;
            else     sum += term;
            neg = !neg;
        }
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    idx_t m = n;
    seq_t result = 1;

    /* handle factor 2 separately */
    if ((m & 1) == 0) {          /* m % 2 == 0 */
        while ((m & 1) == 0) {   /* divide out all 2's */
            m >>= 1;
        }
        /* a(2^e) = 1 so no change to result */
    }

    /* trial division for odd primes up to sqrt(m) */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            result *= prime_power_term(p, e);
        }
    }

    /* remaining prime factor, if any */
    if (m > 1)
        result *= prime_power_term(m, 1);

    return result;
}
