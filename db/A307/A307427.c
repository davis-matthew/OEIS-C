/*
 * A307427  Dirichlet g.f.: zeta(3*s) / (zeta(s) * zeta(2*s)).
 * https://oeis.org/A307427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = ∏_{p^e||n} ( 2 if e≡0 (mod 3) else –1 ), with a(1)=1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    seq_t res = 1;

    /* a(1) = 1, already handled by the loop as n will not have factors. */
    idx_t m = n;

    /* Factor n by trial division. */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e % 3 == 0) {
                res *= (seq_t)2;
            } else {
                res *= (seq_t)-1;
            }
        }
    }

    /* If a prime factor greater than sqrt(n) remains. */
    if (m > 1) {
        /* Remaining prime has exponent 1 ≡ 1 (mod 3). */
        res *= (seq_t)-1;
    }

    return res;
}
