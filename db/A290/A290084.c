/*
 * A290084  a(1) = a(2) = 1; for n > 2, a(n) = A000010(n) / A270492(n).
 * https://oeis.org/A290084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 1; for n > 2, a(n) = A000010(n) / A270492(n)
 * Range:     u8 n <= 582, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9961 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A290084_gcd(idx_t x, idx_t y)
{
    while (y) {
        idx_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    idx_t phi = 1;
    idx_t g = 0;
    idx_t m = n;
    idx_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            idx_t e = 0;
            idx_t pk = 1;
            while (m % p == 0) {
                m /= p;
                e++;
                if (e > 1)
                    pk *= p;
            }
            idx_t ph = (p - 1) * pk;
            phi *= ph;
            if (p == 2) {
                if (e == 2) {
                    g = A290084_gcd(g, 2);
                } else if (e >= 3) {
                    g = A290084_gcd(g, 2);
                    g = A290084_gcd(g, (idx_t)1 << (e - 2));
                }
            } else {
                g = A290084_gcd(g, ph);
            }
        }
    }
    if (m > 1) {
        phi *= (m - 1);
        if (m != 2)
            g = A290084_gcd(g, m - 1);
    }
    return (seq_t)(phi / g);
}
