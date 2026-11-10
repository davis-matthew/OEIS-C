/*
 * A364255  a(n) = gcd(n, A163511(n)).
 * https://oeis.org/A364255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A163511(n)); A163511(n) = p(r) * Prod_{i=1..r}
 *            p(i)^(g_i), with g_i the gap of 0-bits before the i-th set bit
 *            of n and r = A000120(n). The gcd is computed prime by prime as
 *            the minimum of exponents
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 122 terms through n = 9259 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    uint32_t pr[70];
    int np = 0;
    for (uint32_t c = 2; np < 66; c++) {
        int ok = 1;
        for (int j = 0; j < np && pr[j] * pr[j] <= c; j++) {
            if (c % pr[j] == 0) { ok = 0; break; }
        }
        if (ok) pr[np++] = c;
    }

    int r = 0;
    for (idx_t t = n; t; t >>= 1) if (t & 1) r++;

    seq_t result = (seq_t)1;
    idx_t prev = 0, bitpos = 1, t = n;
    int i = 0;
    while (t) {
        if (t & 1) {
            idx_t e = bitpos - prev - 1;
            if (i + 1 == r) e += 1;
            prev = bitpos;
            uint64_t p = pr[i];
            idx_t v = 0;
            idx_t m = n;
            while (v < e && m % p == 0) { m /= p; v++; }
            for (idx_t k = 0; k < v; k++) result *= (seq_t)p;
            i++;
        }
        bitpos++;
        t >>= 1;
    }
    return result;
}
