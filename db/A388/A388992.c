/*
 * A388992  a(n) is the signed denominator of ratio A326128(n) / A326127(n),
 *          with a(6) = 0 by convention.
 * https://oeis.org/A388992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = A326127(n) / gcd(A326128(n), |A326127(n)|), where
 *            A326128(n) = n - A007913(n), A326127(n) = sigma(n) - A007913(n)
 *            - n; a(6) = 0
 * Range:     i8 n <= 107, i16 n <= 14399, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 100 terms through n = 7588 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t a388992_gcd(seq_t x, seq_t y)
{
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t sigma = 1;
    seq_t sqfree = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p != 0) continue;
        int e = 0;
        seq_t sum = 1, pw = 1;
        while (m % p == 0) {
            m /= p;
            e++;
            pw *= (seq_t)p;
            sum += pw;
        }
        sigma *= sum;
        if (e & 1) sqfree *= (seq_t)p;
    }
    if (m > 1) {
        sigma *= (1 + (seq_t)m);
        sqfree *= (seq_t)m;
    }

    seq_t A = (seq_t)n - sqfree;
    seq_t B = sigma - sqfree - (seq_t)n;

    if (B == 0) return 0;
    seq_t g = a388992_gcd(A, B);
    return B / g;
}
