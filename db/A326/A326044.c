/*
 * A326044  a(n) = n - {the largest square dividing its sum of divisors}: a(n)
 *          = n - A008833(sigma(n)).
 * https://oeis.org/A326044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - largest square divisor of sigma(n)
 * Range:     i8 n <= 133, i16 n <= 9362, i32 n <= 9362, i64 n <= 9362,
 *            i128 n <= 9362, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*  Compute a(n) = n - A008833(sigma(n))
    sigma(n) = sum of divisors of n
    A008833(m) = largest square dividing m
*/
seq_t a(idx_t n)
{
    /* ---------------------------------
       1. Compute sigma(n) as uint64_t.
       --------------------------------- */
    uint64_t temp = (uint64_t)n;
    uint64_t sigma = 1;              /* product of (p^(e+1)-1)/(p-1) */

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t term   = 1;     /* (p^(e+1)-1)/(p-1) */
            uint64_t power  = 1;
            while (temp % p == 0) {
                temp  /= p;
                power *= p;
                term  += power;     /* sum of powers of p */
            }
            sigma *= term;
        }
    }
    if (temp > 1) {                   /* remaining prime factor */
        sigma *= (1 + temp);
    }

    /* ---------------------------------
       2. Find largest square dividing sigma.
       --------------------------------- */
    uint64_t tmp   = sigma;
    uint64_t sqdiv = 1;              /* largest square divisor */

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int exp = 0;              /* exponent of p in sigma */
            while (tmp % p == 0) {
                tmp  /= p;
                exp  += 1;
            }
            int pairs = exp / 2;      /* number of p^2 factors */
            for (int i = 0; i < pairs; ++i) {
                sqdiv *= p * p;
            }
        }
    }
    /* if tmp > 1, it is a prime with exponent 1 => contributes no square factor */

    /* ---------------------------------
       3. Return a(n).
       --------------------------------- */
    return (seq_t)n - (seq_t)sqdiv;
}
