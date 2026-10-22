/*
 * A317529  Expansion of Sum_{k>=1} x^(k^2)/(1 + x^(k^2)).
 * https://oeis.org/A317529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) is a multiplicative function defined by a(1) = 1 a(2^e) =
 *            -⌊e/2 + 1⌋ for odd e, a(2^e) = -⌊(e-1)/2⌋ for even e, a(p^e) =
 *            ⌊e/2 + 1⌋ for odd primes p. Then a(n*m)=a(n)*a(m) when
 *            gcd(n,m)=1
 * Range:     i8 n <= 9073, i16 n <= 9073, i32 n <= 9073, i64 n <= 9073,
 *            i128 n <= 9073, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety, although n>=1 per OEIS */

    uint64_t m = (uint64_t)n;      /* work in unsigned for factorisation */
    seq_t result = 1;              /* identity for multiplicative function */

    /* factor 2 separately */
    uint64_t e = 0;
    while ((m & 1ULL) == 0ULL) {
        m >>= 1;
        ++e;
    }
    if (e > 0) {
        seq_t val;
        if (e & 1ULL) {                   /* odd exponent */
            val = -(seq_t)(e / 2ULL + 1);
        } else {                           /* even exponent */
            val = -(seq_t)((e / 2ULL) - 1);
            /* (e-1)/2 == e/2 - 1 when e even */
        }
        result *= val;
    }

    /* trial division on odd factors */
    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL) {
        if (m % p == 0ULL) {
            uint64_t exp = 0ULL;
            while (m % p == 0ULL) {
                m /= p;
                ++exp;
            }
            seq_t val = (seq_t)(exp / 2ULL + 1);
            result *= val;
        }
    }

    /* if something remains, it's an odd prime with exponent 1 */
    if (m > 1ULL) {
        /* exponent 1 → val = floor(1/2 + 1) = 1 */
        result *= (seq_t)1;
    }

    return result;
}
