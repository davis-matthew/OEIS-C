/*
 * A369100  Dirichlet g.f.: zeta(s)^3 * (1 - 2^(1-s))^2.
 * https://oeis.org/A369100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = product_{p^e||n} f(p,e) * where f(2,e) = (e^2-5e+2)/2, *
 *            f(p odd,e) = (e+1)*(e+2)/2 * * The sequence is multiplicative
 *            with the above local factors
 * Range:     i8 n <= 2834, i16 n <= 9516, i32 n <= 9516, i64 n <= 9516,
 *            i128 n <= 9516, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */
    seq_t result = 1;
    idx_t m = n;
    /* factor out powers of 2 */
    idx_t e2 = 0;
    while ((m & 1ULL) == 0) {
        ++e2;
        m >>= 1;
    }
    if (e2 > 0) {
        seq_t val = ((seq_t)e2 * (seq_t)e2 - (seq_t)5 * (seq_t)e2 + (seq_t)2) / (seq_t)2;
        result *= val;
    }

    /* factor odd primes by trial division */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            seq_t val = ((seq_t)(e + 1) * (seq_t)(e + 2)) / (seq_t)2;
            result *= val;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {           /* m is an odd prime with exponent 1 */
        result *= (seq_t)3;  /* (1+1)*(1+2)/2 = 3 */
    }

    return result;
}
