/*
 * A347133  a(n) = Sum_{d|n} A003415(n/d) * A069359(d).
 * https://oeis.org/A347133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A003415(n/d) * A069359(d)
 * Range:     u8 n <= 47, u16 n <= 1919, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 108 terms through n = 9869 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A347133_deriv(idx_t m) {
    if (m <= 1) return 0;
    seq_t res = 0;
    idx_t r = m;
    for (idx_t p = 2; p <= r / p; p++) {
        if (r % p == 0) {
            unsigned e = 0;
            while (r % p == 0) { r /= p; e++; }
            res += (seq_t)(m / p) * (seq_t)e;
        }
    }
    if (r > 1) res += (seq_t)(m / r);
    return res;
}

static seq_t A347133_s(idx_t m) {
    if (m <= 1) return 0;
    seq_t res = 0;
    idx_t r = m;
    for (idx_t p = 2; p <= r / p; p++) {
        if (r % p == 0) {
            res += (seq_t)(m / p);
            while (r % p == 0) r /= p;
        }
    }
    if (r > 1) res += (seq_t)(m / r);
    return res;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t i = 1; i <= n / i; i++) {
        if (n % i == 0) {
            idx_t j = n / i;
            total += A347133_deriv(j) * A347133_s(i);
            if (j != i)
                total += A347133_deriv(i) * A347133_s(j);
        }
    }
    return total;
}
