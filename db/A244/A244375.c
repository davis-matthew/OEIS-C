/*
 * A244375  Expansion of (a(q) + 3*a(q^2) - 4*a(q^4)) / 6 in powers of q where
 *          a() is a cubic AGM theta function.
 * https://oeis.org/A244375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:   Multiplicative definition
 * a(n) = Product over prime powers p^e || n of:
 *   if p == 2 : 3 * (-1)^(e+1)
 *   if p == 3 : 1
 *   if p ≡ 1 (mod 6) : e+1
 *   if p ≡ 5 (mod 6) : (1 + (-1)^e)/2
 *   otherwise        : 1
 * (If a prime power is the only factor, e = 1.)
 * The function factors n by trial division (sufficient for n ≤ 10000).
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset 1 not used, but safe */

    seq_t prod = 1;
    idx_t m = n;

    /* factor 2 */
    unsigned e2 = 0;
    while ((m & 1ULL) == 0) {            /* m % 2 == 0 */
        m >>= 1;
        e2++;
    }
    if (e2 > 0) {
        seq_t sign = ((e2 + 1) & 1) ? -1 : 1;
        prod *= (seq_t)(3 * sign);
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }

            seq_t val;
            if (p % 6 == 1) {
                val = (seq_t)(e + 1);
            } else if (p % 6 == 5) {
                seq_t t = (e & 1) ? -1 : 1;
                val = (seq_t)((1 + t) / 2);
            } else {
                val = 1;                 /* should not happen for primes > 3 */
            }
            prod *= val;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        idx_t p = m;
        unsigned e = 1; /* exponent is 1 for a remaining prime */

        seq_t val;
        if (p == 2) {
            seq_t sign = ((e + 1) & 1) ? -1 : 1;
            val = (seq_t)(3 * sign);
        } else if (p == 3) {
            val = 1;
        } else if (p % 6 == 1) {
            val = (seq_t)(e + 1);
        } else if (p % 6 == 5) {
            seq_t t = (e & 1) ? -1 : 1;
            val = (seq_t)((1 + t) / 2);
        } else {
            val = 1;
        }
        prod *= val;
    }

    return prod;
}
