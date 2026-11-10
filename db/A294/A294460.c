/*
 * A294460  E.g.f.: exp(-Sum_{n>=1} A000593(n) * x^n).
 * https://oeis.org/A294460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(n) = -(n-1)! * Sum_{k=1..n}
 *            k*A000593(k)*a(n-k)/(n-k)! for n > 0
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 30,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 428 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t A000593_a(idx_t n) {
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
    }
    seq_t result = 0;
    for (idx_t d = 1; d <= m; ++d) {
        if (m % d == 0) {
            result += (seq_t)d;
            idx_t complement = m / d;
            if (d < complement) {
                result += (seq_t)complement;
            }
        }
        if (d > m / d) break;
    }
    return result;
}

static seq_t *A294460_tab = 0;
static idx_t A294460_cnt = 0;
static idx_t A294460_cap = 0;

seq_t a(idx_t n) {
    if (n >= A294460_cnt) {
        if (n + 1 > A294460_cap) {
            idx_t ncap = (n + 1 > 2 * A294460_cap) ? n + 1 : 2 * A294460_cap;
            seq_t *nt = mem_new(seq_t, (size_t)ncap);
            for (idx_t i = 0; i < A294460_cnt; ++i) nt[i] = A294460_tab[i];
            if (A294460_tab) mem_free(A294460_tab);
            A294460_tab = nt;
            A294460_cap = ncap;
        }
        if (A294460_cnt == 0) {
            A294460_tab[0] = 1;
            A294460_cnt = 1;
        }
        for (idx_t i = A294460_cnt; i <= n; ++i) {
            seq_t sum = 0;
            seq_t P = 1; /* (i-1)!/(i-k)! */
            for (idx_t k = 1; k <= i; ++k) {
                sum += (seq_t)k * A000593_a(k) * A294460_tab[i - k] * P;
                P *= (seq_t)(i - k);
            }
            A294460_tab[i] = -sum;
        }
        A294460_cnt = n + 1;
    }
    return A294460_tab[n];
}
