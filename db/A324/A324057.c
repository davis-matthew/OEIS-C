/*
 * A324057  a(n) = A106315(A005940(1+n)).
 * https://oeis.org/A324057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A106315(A005940(1+n)) = (m*d(m)) mod sigma(m), m =
 *            A005940(1+n); d and sigma are computed from the prime exponents
 *            of m given by the binary expansion of n
 * Range:     u8 n <= 51, u16 n <= 487, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 104 terms through n = 7492 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    unsigned e[72];
    unsigned long pr[72];
    int i, cnt = 0;
    for (i = 0; i < 72; i++) e[i] = 0;
    for (unsigned long c = 2; cnt < 72; c++) {
        int ok = 1;
        for (int j = 0; j < cnt && pr[j] * pr[j] <= c; j++)
            if (c % pr[j] == 0) { ok = 0; break; }
        if (ok) pr[cnt++] = c;
    }

    idx_t mm = n;
    int k = 0; /* zero-based prime index */
    while (mm > 0) {
        if (mm & 1) {
            e[k]++;
            mm = (mm - 1) / 2;
        } else {
            mm /= 2;
            k++;
        }
    }

    seq_t m = 1, tau = 1, sigma = 1;
    for (i = 0; i < 72; i++) {
        if (e[i] == 0) continue;
        seq_t p = (seq_t)pr[i];
        seq_t term = 1, sum = 1;
        for (unsigned j = 0; j < e[i]; j++) {
            term *= p;
            sum += term;
        }
        m *= term;
        tau *= (seq_t)(e[i] + 1);
        sigma *= sum;
    }
    return (m * tau) % sigma;
}
