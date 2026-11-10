/*
 * A294462  Expansion of e.g.f. Product_{k>0} (1-k*x^k)^(-1/k).
 * https://oeis.org/A294462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = (n-1)! * Sum_{k=1..n} A055225(k)*a(n-k)/(n-k)!
 *            for n > 0; A055225(m) = Sum_{d|m} (m/d)^d
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 392 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

static seq_t A055225_pow(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

static seq_t A055225_a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t k1 = d;
            sum += A055225_pow((seq_t)n / (seq_t)k1, k1);
            idx_t k2 = n / d;
            if (k2 != k1) sum += A055225_pow((seq_t)n / (seq_t)k2, k2);
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t *c = mem_new(seq_t, (size_t)n + 1);
    seq_t *t = mem_new(seq_t, (size_t)n + 1);
    for (idx_t k = 1; k <= n; ++k) c[k] = A055225_a(k);
    t[0] = 1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t s = 0;
        seq_t f = 1; /* (m-1)!/(m-k)! */
        for (idx_t k = 1; k <= m; ++k) {
            if (k > 1) f *= (seq_t)(m - k + 1);
            s += f * c[k] * t[m - k];
        }
        t[m] = s;
    }
    seq_t r = t[n];
    mem_free(c);
    mem_free(t);
    return r;
}
