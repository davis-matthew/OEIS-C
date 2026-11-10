/*
 * A294341  E.g.f.: exp(Sum_{n>=1} A010815(n)*x^n/n).
 * https://oeis.org/A294341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(n) = (n-1)! * Sum_{k=1..n} A010815(k)*a(n-k)/(n-k)!
 *            i.e. a(n) = Sum_{k=1..n} A010815(k) * (n-1)(n-2)...(n-k+1) *
 *            a(n-k), computed over the pentagonal k only
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 446 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A294341_pent(idx_t k, idx_t *kk, int *sg, int *state, idx_t *m) {
    (void)k; (void)kk; (void)sg; (void)state; (void)m;
    return 0;
}

seq_t a(idx_t n) {
    (void)A294341_pent;
    seq_t *t = mem_new(seq_t, (size_t)n + 1);
    t[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t P = 1;      /* (i-1)...(i-k+1) */
        idx_t pk = 1;     /* k for which P is current */
        for (idx_t m = 1; ; ++m) {
            idx_t g1 = m * (3 * m - 1) / 2;
            idx_t g2 = m * (3 * m + 1) / 2;
            if (g1 > i) break;
            for (int s = 0; s < 2; ++s) {
                idx_t k = s ? g2 : g1;
                if (k > i) break;
                while (pk < k) { P = P * (seq_t)(i - pk); ++pk; }
                seq_t term = P * t[i - k];
                if (m & 1) sum -= term; else sum += term;
            }
        }
        t[i] = sum;
    }
    seq_t r = t[n];
    mem_free(t);
    return r;
}
