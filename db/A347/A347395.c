/*
 * A347395  Dirichlet convolution of Liouville's lambda (A008836) with
 *          A342001, where A342001(n) = A003415(n)/A003557(n).
 * https://oeis.org/A347395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Sum_{d|n} A008836(n/d) * A342001(d), where A342001(d) =
 *            Sum_{p^v || d} v*rad(d)/p
 * Range:     u8 n <= 513, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 109 terms through n = 9479 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t pr[24];
    int ex[24];
    int k = 0;
    idx_t t = n;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; ++e; }
            pr[k] = p; ex[k] = e; ++k;
        }
    }
    if (t > 1) { pr[k] = t; ex[k] = 1; ++k; }

    int f[24];
    int total = 0;
    for (int i = 0; i < k; ++i) { f[i] = 0; total += ex[i]; }

    seq_t pos = 0, neg = 0;
    for (;;) {
        /* current divisor d with exponents f[] */
        int sumf = 0;
        seq_t rad = 1;
        for (int i = 0; i < k; ++i) {
            if (f[i] > 0) rad *= (seq_t)pr[i];
            sumf += f[i];
        }
        seq_t val = 0;
        for (int i = 0; i < k; ++i) {
            if (f[i] > 0) val += (seq_t)f[i] * (rad / (seq_t)pr[i]);
        }
        if (((total - sumf) & 1) == 0) pos += val; else neg += val;

        int i = 0;
        while (i < k) {
            if (f[i] < ex[i]) { ++f[i]; break; }
            f[i] = 0; ++i;
        }
        if (i == k) break;
    }
    return pos - neg;
}
