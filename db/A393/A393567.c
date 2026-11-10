/*
 * A393567  Coefficients of the formal power series quotient of the
 *          '5th-order' mock theta function f_0(q) by the 3rd-order mock theta
 *          function f(q).
 * https://oeis.org/A393567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   A(q) = f_0(q)/f(q), f_0 = Sum_{n>=0} q^(n^2)/(-q;q)_n, f =
 *            Sum_{n>=0} q^(n^2)/(-q;q)_n^2; a(0)=1, a(n) = A053256(n) -
 *            Sum_{k=1..n} A000025(k)*a(n-k)
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 24, i64 n <= 48, i128 n <= 95,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 1839 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *Q_tab = 0;
static size_t Q_len = 0;

static void Q_extend(size_t need)
{
    size_t N, k, m, i;
    seq_t *g, *h, *f0, *f, *r;
    if (need <= Q_len) return;
    if (need < 2 * Q_len) need = 2 * Q_len;
    N = need - 1;

    g = mem_new(seq_t, need);
    h = mem_new(seq_t, need);
    f0 = mem_new(seq_t, need);
    f = mem_new(seq_t, need);
    r = mem_new(seq_t, need);

    g[0] = 1;
    h[0] = 1;
    f0[0] = 1;
    f[0] = 1;
    for (k = 1; k * k <= N; k++) {
        size_t k2 = k * k;
        for (m = k; m <= N; m++) g[m] -= g[m - k];
        for (m = k; m <= N; m++) h[m] -= h[m - k];
        for (m = k; m <= N; m++) h[m] -= h[m - k];
        for (i = k2; i <= N; i++) {
            f0[i] += g[i - k2];
            f[i] += h[i - k2];
        }
    }

    for (m = 0; m <= N; m++) {
        seq_t s = f0[m];
        for (k = 1; k <= m; k++) s -= f[k] * r[m - k];
        r[m] = s;
    }

    mem_free(g);
    mem_free(h);
    mem_free(f0);
    mem_free(f);
    if (Q_tab) mem_free(Q_tab);
    Q_tab = r;
    Q_len = need;
}

seq_t a(idx_t n)
{
    Q_extend((size_t)n + 1);
    return Q_tab[n];
}
