/*
 * A260658  Numerators of a BBP-like formula for 4*Pi/sqrt(27).
 * https://oeis.org/A260658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator((-1/8)^n*(2/(3*n+1)+1/(3*n+2)))
 * Range:     i8 n <= 13, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   mod_pow8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow8.h"

static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Compute h = 9*n + 5 */
    seq_t h = (seq_t)9 * (seq_t)n + 5;

    /* Compute the denominator modulo h: (3n+1)*(3n+2)*8^n mod h */
    seq_t a1 = (seq_t)3 * (seq_t)n + 1;
    seq_t a2 = (seq_t)3 * (seq_t)n + 2;
    seq_t mod = h;

    seq_t a1_mod = a1 % mod;
    seq_t a2_mod = a2 % mod;

    seq_t mul = (a1_mod * a2_mod) % mod;
    seq_t pow8 = mod_pow8(n, mod);
    seq_t denom_mod = (mul * pow8) % mod;

    /* GCD of h and the denominator modulo h gives the gcd of the fraction */
    seq_t g = seq_gcd(h, denom_mod);

    /* Simplified numerator = ((-1)^n) * (h / g) */
    seq_t num = h / g;
    if (n & 1)
        num = -num;

    return num;
}
