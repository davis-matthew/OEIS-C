/*
 * A308518  Expansion of e.g.f. exp(1 - exp(1 - exp(x))).
 * https://oeis.org/A308518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{k=1..n} binomial(n-1,k-1)*A000587(k)*a(n-k), a(0) =
 *            1; A000587(n+1) = -Sum_{i=0..n} binomial(n,i)*A000587(i),
 *            A000587(0) = 1
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 16, i64 n <= 25, i128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *e_a = NULL;
static seq_t *e_b = NULL;
static idx_t e_cnt = 0;

static void e_ensure(idx_t n) {
    idx_t m, j;
    if (e_cnt > n) return;
    if (e_cnt == 0) {
        e_a = mem_new(seq_t, (size_t)(n + 1));
        e_b = mem_new(seq_t, (size_t)(n + 1));
    } else {
        e_a = mem_resize(seq_t, e_a, (size_t)(n + 1));
        e_b = mem_resize(seq_t, e_b, (size_t)(n + 1));
    }
    for (m = e_cnt; m <= n; m++) {
        if (m == 0) {
            e_b[0] = 1;
            e_a[0] = 1;
            continue;
        }
        seq_t c = 1;
        seq_t sb = 0;
        for (j = 0; j < m; j++) {
            sb += c * e_b[j];
            c = c * ((seq_t)(m - 1) - (seq_t)j) / ((seq_t)j + 1);
        }
        e_b[m] = -sb;
        c = 1;
        seq_t sa = 0;
        for (j = 0; j < m; j++) {
            sa += c * e_b[j + 1] * e_a[m - 1 - j];
            c = c * ((seq_t)(m - 1) - (seq_t)j) / ((seq_t)j + 1);
        }
        e_a[m] = -sa;
    }
    e_cnt = n + 1;
}

seq_t a(idx_t n) {
    e_ensure(n);
    return e_a[n];
}
