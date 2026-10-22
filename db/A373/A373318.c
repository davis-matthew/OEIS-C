/*
 * A373318  Numerator of the asymptotic density of numbers that are unitarily
 *          divided by n.
 * https://oeis.org/A373318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of the asymptotic density of numbers that are
 *            unitarily divided by n. The density is f(n) = ∏_{p^e‖n} (p-1) /
 *            p^{e+1}, reduced to lowest terms. The function returns the
 *            reduced numerator
 * Range:     u8 n <= 18, u16 n <= 380, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t seq_pow_simple(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    idx_t m = n;
    seq_t numerator  = (seq_t)1;
    seq_t denominator = (seq_t)1;

    for (idx_t p = (idx_t)2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t sp = (seq_t)p;
            numerator *= (sp - (seq_t)1);
            denominator *= seq_pow_simple(sp, e + (idx_t)1);
        }
    }

    if (m > (idx_t)1) {           /* remaining prime factor */
        seq_t sp = (seq_t)m;
        numerator *= (sp - (seq_t)1);  /* exponent e = 1 */
        denominator *= seq_pow_simple(sp, (idx_t)2);
    }

    seq_t g = gcd(numerator, denominator);
    numerator /= g;   /* return reduced numerator */
    return numerator;
}
