/*
 * A388991  a(n) is the numerator of ratio A326128(n) / A326127(n), with a(6)
 *          = 0 by convention.
 * https://oeis.org/A388991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (n - A007913(n)) / (sigma(n) - A007913(n) -
 *            n), with a(6) = 0
 * Range:     u8 n <= 287, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9718 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sigma_v = 1;
    seq_t sqfree = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p != 0) continue;
        int e = 0;
        seq_t pp = 1, s = 1;
        while (m % p == 0) {
            m /= p;
            e++;
            pp *= (seq_t)p;
            s += pp;
        }
        sigma_v *= s;
        if (e & 1) sqfree *= (seq_t)p;
    }
    if (m > 1) {
        sigma_v *= ((seq_t)m + 1);
        sqfree *= (seq_t)m;
    }

    seq_t num = (seq_t)n - sqfree;
    if (num == 0) return 0;

    seq_t other = sqfree + (seq_t)n;
    seq_t den;
    if (sigma_v >= other) den = sigma_v - other;
    else den = other - sigma_v;
    if (den == 0) return 0;

    seq_t g = gcd(num, den);
    return num / g;
}
