/*
 * A326938  Dirichlet g.f.: 1 / (zeta(s) * zeta(s-1) * (1 - 2^(-s))).
 * https://oeis.org/A326938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 92, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A326938: Dirichlet inverse of A002131 (multiplicative).
   a(1) = 1; a(2^e) = -2 for e=1, 0 otherwise;
   a(p^e) = -(p+1) for e=1, p for e=2, 0 for e>2 (p odd prime). */

#include <stdint.h>    /* for uint64_t */

static inline idx_t spf(idx_t n) {
    /* smallest prime factor of n, or n if n is prime or 1 */
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) return p;
    }
    return n;
}

seq_t a(idx_t n) {
    if (n < 1) return 0; /* safety, but n>=1 per OEIS */
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    while (m > 1) {
        idx_t p = spf(m);
        idx_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        seq_t val;
        if (p == 2) {
            val = (exp == 1) ? (seq_t)-2 : (seq_t)0;
        } else { /* odd prime */
            if (exp == 1) {
                val = -(seq_t)(p + 1);
            } else if (exp == 2) {
                val = (seq_t)p;
            } else {
                val = (seq_t)0;
            }
        }

        if (val == 0) {
            return (seq_t)0;
        }
        result *= val;
    }
    return result;
}
