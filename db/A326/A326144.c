/*
 * A326144  a(n) = gcd(A066503(n), A326143(n)) = gcd(n - A007947(n), sigma(n)
 *          - A007947(n) - n).
 * https://oeis.org/A326144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n - A007947(n), |sigma(n) - A007947(n) - n|), where
 *            A007947 = rad(n)
 * Range:     u8 n <= 256, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 108 terms through n = 9483 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A326144_gcd(seq_t x, seq_t y)
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
    if (n == 0) return 0;
    seq_t sigma = 1;
    seq_t rad = 1;
    idx_t m = n;
    idx_t p;

    for (p = 2; p <= m / p; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            sigma *= term;
            rad *= (seq_t)p;
        }
    }
    if (m > 1) {
        sigma *= ((seq_t)m + 1);
        rad *= (seq_t)m;
    }

    seq_t u = (seq_t)n - rad;
    seq_t s = rad + (seq_t)n;
    seq_t d = (sigma >= s) ? (sigma - s) : (s - sigma);
    return A326144_gcd(u, d);
}
