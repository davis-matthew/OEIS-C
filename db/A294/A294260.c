/*
 * A294260  E.g.f.: exp(Sum_{n>=1} A010815(n)*x^n).
 * https://oeis.org/A294260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(n) = (n-1)! * Sum_{k=1..n}
 *            k*A010815(k)*a(n-k)/(n-k)! for n > 0; computed as a(n) = Sum_k
 *            k*A010815(k)*(n-1)(n-2)...(n-k+1)*a(n-k), A010815(k) nonzero
 *            only at pentagonal k
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 353 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A294260_memo = 0;
static idx_t A294260_cnt = 0;   /* number of computed terms */
static idx_t A294260_cap = 0;

seq_t a(idx_t n) {
    if (n >= A294260_cap) {
        idx_t ncap = n + 16;
        if (ncap < 2 * A294260_cap) ncap = 2 * A294260_cap;
        if (A294260_memo == 0) {
            A294260_memo = mem_new(seq_t, (size_t)ncap);
        } else {
            A294260_memo = mem_resize(seq_t, A294260_memo, (size_t)ncap);
        }
        A294260_cap = ncap;
    }
    if (A294260_cnt == 0) {
        A294260_memo[0] = (seq_t)1;
        A294260_cnt = 1;
    }
    for (idx_t i = A294260_cnt; i <= n; ++i) {
        seq_t sum = 0;
        seq_t f = 1;      /* (i-1)(i-2)...(i-kk+1) */
        idx_t kk = 1;
        for (idx_t m = 1; ; ++m) {
            idx_t g1 = m * (3 * m - 1) / 2;
            idx_t g2 = m * (3 * m + 1) / 2;
            if (g1 > i) break;
            for (int t = 0; t < 2; ++t) {
                idx_t g = t == 0 ? g1 : g2;
                if (g > i) break;
                while (kk < g) {
                    f *= (seq_t)(i - kk);
                    ++kk;
                }
                seq_t term = (seq_t)g * f * A294260_memo[i - g];
                if (m & 1) sum -= term; else sum += term;
            }
        }
        A294260_memo[i] = sum;
        A294260_cnt = i + 1;
    }
    return A294260_memo[n];
}
