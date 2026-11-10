/*
 * A349619  Dirichlet convolution of A003415 with the Dirichlet inverse of
 *          A003557.
 * https://oeis.org/A349619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A003415(n/d) * A349340(d), where A349340 is
 *            multiplicative with A349340(p^e) = -(p-1)^(e-1)
 * Range:     u8 n <= 302, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 104 terms through n = 9652 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A003415_a(idx_t n) {
    if (n <= 1) return 0;
    seq_t result = 0;
    seq_t orig = (seq_t)n;
    seq_t rem = orig;
    for (seq_t p = 2; p * p <= rem; p++) {
        int e = 0;
        while (rem % p == 0) { rem /= p; e++; }
        if (e > 0) result += (orig / p) * (seq_t)e;
    }
    if (rem > 1) result += orig / rem;
    return result;
}

/* magnitude of A349340(d); *neg set if value is negative */
static seq_t A349340_mag(idx_t d, bool *neg) {
    seq_t res = 1;
    bool ng = false;
    idx_t t = d;
    for (idx_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; e++; }
            ng = !ng;
            for (int i = 1; i < e; i++) res *= (seq_t)(p - 1);
        }
    }
    if (t > 1) ng = !ng;
    *neg = ng;
    return res;
}

seq_t a(idx_t n) {
    seq_t pos = 0, neg = 0;
    for (idx_t d = 1; d <= n; d++) {
        if (n % d != 0) continue;
        seq_t dv = A003415_a(n / d);
        if (dv == 0) continue;
        bool ng;
        seq_t m = A349340_mag(d, &ng);
        if (ng) neg += dv * m;
        else pos += dv * m;
    }
    return pos - neg;
}
