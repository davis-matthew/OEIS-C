/*
 * A307423  Dirichlet g.f.: zeta(2*s) / zeta(3*s).
 * https://oeis.org/A307423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = multiplicative with a(p)=0, a(p^e)=(-1)^e for e>=2
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    idx_t t = n;
    int parity = 0;          /* 0 → even number of odd exponents (≠1) */

    for (idx_t p = 2; p * p <= t; ++p) {
        int exp = 0;
        while (t % p == 0) {
            t /= p;
            ++exp;
        }
        if (exp) {
            if (exp == 1) {  /* exponent 1 → a(p)=0 */
                return (seq_t)0;
            }
            if (exp & 1) {   /* flip parity for odd exponent */
                parity ^= 1;
            }
        }
    }

    /* Remaining factor > 1 is a prime with exponent 1 (if any). */
    if (t > 1) {
        return (seq_t)0;
    }

    return (parity == 0) ? (seq_t)1 : (seq_t)-1;
}
