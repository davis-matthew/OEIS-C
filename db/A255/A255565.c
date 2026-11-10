/*
 * A255565  a(0) = 0; for n >= 1: if n = A255411(k) for some k, then a(n) =
 *          2*a(k), otherwise, n = A256450(h) for some h, and a(n) = 1 +
 *          2*a(h).
 * https://oeis.org/A255565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; for n >= 1: if A257680(n) = 0 then a(n) =
 *            2*a(A257685(n)) [n = A255411(k), k found by shifting the
 *            factorial-base digits of n right and decrementing the nonzero
 *            ones], otherwise a(n) = 1 + 2*a(A273662(n)) [n = A256450(h), h =
 *            n - #{m < n : m has no digit 1 in factorial base}]
 * Range:     u8 n <= 18, u16 n <= 82, u32 n <= 958, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Validated: standard, pass, 89 terms through n = 4942 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void A255565_step(idx_t n, idx_t *next, int *bit)
{
    int d[32];
    idx_t f[22];
    int L = 0, i, has1 = 0;
    idx_t t = n;
    f[0] = 1;
    for (i = 1; i < 21; i++) f[i] = f[i - 1] * (idx_t)i;
    while (t > 0) {
        idx_t base = (idx_t)(L + 2);
        L++;
        d[L] = (int)(t % base);
        t /= base;
        if (d[L] == 1) has1 = 1;
    }
    if (!has1) {
        /* n = A255411(k): shift right, decrement nonzero digits */
        idx_t k = 0;
        int j;
        for (j = L - 1; j >= 1; j--) {
            int e = d[j + 1] > 0 ? d[j + 1] - 1 : 0;
            k = k * (idx_t)(j + 1) + (idx_t)e;
        }
        *next = k;
        *bit = 0;
    } else {
        /* C = count of m in [0, n) with no digit 1 in factorial base */
        idx_t C = 0;
        for (i = L; i >= 1; i--) {
            idx_t c = (idx_t)(d[i] > 1 ? d[i] - 1 : d[i]);
            C += c * f[i - 1];
            if (d[i] == 1) break;
        }
        *next = n - C;
        *bit = 1;
    }
}

seq_t a(idx_t n)
{
    unsigned char bits[256];
    int len = 0;
    seq_t r = 0;
    while (n > 0 && len < 256) {
        idx_t nx;
        int b;
        A255565_step(n, &nx, &b);
        bits[len++] = (unsigned char)b;
        n = nx;
    }
    while (len > 0) {
        len--;
        r = r * 2 + (seq_t)bits[len];
    }
    return r;
}
