/*
 * A371908  a(n) = 2-adic valuation of A008336(2*n).
 * https://oeis.org/A371908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A007814(A008336(2*n)), where A008336(1) = 1 and
 *            A008336(k+1) = A008336(k)/k if k | A008336(k), else
 *            A008336(k)*k. Computed via prime-exponent vectors (no big
 *            numbers)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9882 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t N = 2 * n;
    int *ex = (int *)calloc((size_t)N + 2, sizeof(int));
    if (!ex) abort();
    for (idx_t k = 1; k < N; ++k) {
        /* factor k */
        idx_t ps[24];
        int es[24];
        int cnt = 0;
        idx_t t = k;
        for (idx_t p = 2; p * p <= t; ++p) {
            if (t % p == 0) {
                int e = 0;
                while (t % p == 0) { t /= p; ++e; }
                ps[cnt] = p; es[cnt] = e; ++cnt;
            }
        }
        if (t > 1) { ps[cnt] = t; es[cnt] = 1; ++cnt; }
        int divisible = 1;
        for (int i = 0; i < cnt; ++i) {
            if (ex[ps[i]] < es[i]) { divisible = 0; break; }
        }
        for (int i = 0; i < cnt; ++i) {
            if (divisible) ex[ps[i]] -= es[i];
            else ex[ps[i]] += es[i];
        }
    }
    seq_t r = (seq_t)ex[2];
    free(ex);
    return r;
}
