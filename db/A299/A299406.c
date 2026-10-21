/*
 * A299406  Dirichlet g.f.: Sum_{n>0} a(n)/n^s =
 *          (zeta(s)*zeta(6*s))/(zeta(2*s)*zeta(3*s)).
 * https://oeis.org/A299406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative with prime power rule a(p^e) = (-1)^( (e
 *            mod 3) + (e mod 6) ) if e mod 3 < 2, 0 otherwise. Offset: 1
 *            (a(1) = 1)
 * Range:     i8 n <= 9479, i16 n <= 9479, i32 n <= 9479, i64 n <= 9479,
 *            i128 n <= 9479, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Evaluate a(p^e) for e >= 1 */
static seq_t prime_power_value(idx_t e)
{
    if ((e % 3) == 2)          /* e mod 3 < 2 fails -> 0 */
        return 0;
    idx_t r1 = e % 3;          /* 0 or 1 */
    idx_t r2 = e % 6;          /* 0 .. 5 */
    idx_t sum = r1 + r2;
    return (sum & 1) ? -1 : 1; /* (-1)^sum */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined but return 0 */
        return 0;
    if (n == 1)            /* a(1) = 1 */
        return 1;

    seq_t prod = 1;
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t val = prime_power_value(e);
            if (val == 0)
                return 0;
            prod *= val;
        }
    }

    /* Remaining prime factor larger than sqrt(n) */
    if (temp > 1) {
        seq_t val = prime_power_value(1); /* exponent 1 */
        if (val == 0)
            return 0;
        prod *= val;
    }

    return prod;
}
