/*
 * A348948  a(n) = sigma(n) / gcd(sigma(n), A348944(n)), where A348944 is the
 *          arithmetic mean of A003959 and A034448, and sigma is the sum of
 *          divisors function.
 * https://oeis.org/A348948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = sigma(n) / gcd(sigma(n), (A003959(n)+A034448(n))/2)
 * Range:     u8 n <= 71, u16 n <= 24335, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9298 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A348948_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t sig = 1, f = 1, g = 1;
    uint64_t p;

    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1, pw = 1, fp = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
                fp *= (seq_t)(p + 1);
            }
            sig *= term;
            f *= fp;
            g *= (pw + 1);
        }
    }
    if (m > 1) {
        sig *= ((seq_t)m + 1);
        f *= ((seq_t)m + 1);
        g *= ((seq_t)m + 1);
    }
    /* f and g have the same parity; avoid overflow in f+g */
    seq_t mean = f / 2 + g / 2 + (f & g & 1);
    seq_t d = A348948_gcd(sig, mean);
    return sig / d;
}
