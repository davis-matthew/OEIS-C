/*
 * A328502  Dirichlet g.f.: zeta(s-1) / (zeta(s) * zeta(s-2)).
 * https://oeis.org/A328502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Σ_{d|n} φ(n/d) μ(d) d² * The function implements the
 *            multiplicative definition: * a(1) = 1 * a(p) = p - 1 - p² for
 *            prime p * a(p^e) = -p^(e-1) (p-1)² for e>1 * For any
 *            n=∏p_i^{e_i}, a(n) = ∏ a(p_i^{e_i})
 * Range:     i8 n <= 12, i16 n <= 190, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t remaining = n;

    /* trial division up to sqrt(remaining) */
    for (idx_t p = 2; (uint64_t)p * (uint64_t)p <= remaining; ++p) {
        if (remaining % p == 0) {
            /* count exponent e of prime p */
            int e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++e;
            }

            seq_t term;
            if (e == 1) {
                /* a(p) = p-1-p^2 */
                term = (seq_t)p - 1 - (seq_t)p * (seq_t)p;
            } else {
                /* a(p^e) = -p^(e-1)*(p-1)^2 */
                seq_t pow_p = 1;
                for (int i = 0; i < e - 1; ++i)
                    pow_p *= (seq_t)p;
                seq_t diff = (seq_t)p - 1;
                pow_p *= diff;
                pow_p *= diff;
                term = -pow_p;
            }
            result *= term;
        }
    }

    /* if a prime factor larger than sqrt remains */
    if (remaining > 1) {
        /* remaining is prime, exponent e = 1 */
        seq_t p = (seq_t)remaining;
        seq_t term = p - 1 - p * p;   /* a(p) for e = 1 */
        result *= term;
    }

    return result;
}
