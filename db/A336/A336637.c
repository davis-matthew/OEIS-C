/*
 * A336637  Sum_{n>=0} a(n) * x^n / (n!)^2 = exp(BesselI(0,2*sqrt(x))^4 - 1).
 * https://oeis.org/A336637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (1/n) * Sum_{k=1..n} binomial(n,k)^2 *
 *            A002895(k) * k * a(n-k), with A002895 the Domb numbers
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

static int A002895_fixed_width(void) {
    seq_t x = (seq_t)1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return x == 0;
}

static seq_t A002895_rec(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    seq_t p2 = (seq_t)1, p1 = (seq_t)4;
    idx_t i;
    for (i = 2; i <= n; i++) {
        seq_t ni = (seq_t)i;
        seq_t im = ni - 1;
        seq_t t1 = (seq_t)2 * (2 * ni - 1) * (5 * ni * ni - 5 * ni + 2) * p1;
        seq_t t2 = (seq_t)64 * im * im * im * p2;
        seq_t cur = (t1 - t2) / (ni * ni * ni);
        p2 = p1;
        p1 = cur;
    }
    return p1;
}

static seq_t A002895_sum(idx_t n) {
    seq_t cm = (seq_t)1;
    idx_t j, k;
    for (j = 0; j < n; j++)
        cm = cm * (seq_t)(2 * (2 * j + 1)) / (seq_t)(j + 1);
    seq_t bnk = (seq_t)1;
    seq_t ck = (seq_t)1;
    seq_t sum = (seq_t)0;
    for (k = 0; k <= n; k++) {
        sum += bnk * bnk * cm * ck;
        if (k == n) break;
        bnk = bnk * (seq_t)(n - k) / (seq_t)(k + 1);
        ck = ck * (seq_t)(2 * (2 * k + 1)) / (seq_t)(k + 1);
        {
            idx_t m = n - k;
            cm = cm * (seq_t)m / (seq_t)(2 * (2 * m - 1));
        }
    }
    return sum;
}

static seq_t A002895_a(idx_t n) {
    if (A002895_fixed_width()) return A002895_sum(n);
    return A002895_rec(n);
}

seq_t a(idx_t n) {
    seq_t *D = mem_new(seq_t, (size_t)n + 1);
    seq_t *A = mem_new(seq_t, (size_t)n + 1);
    idx_t m, k;
    for (k = 0; k <= n; k++) D[k] = A002895_a(k);
    A[0] = (seq_t)1;
    for (m = 1; m <= n; m++) {
        seq_t b = (seq_t)1;
        seq_t sum = (seq_t)0;
        for (k = 1; k <= m; k++) {
            b = b * (seq_t)(m - k + 1) / (seq_t)k;
            sum += b * b * D[k] * (seq_t)k * A[m - k];
        }
        A[m] = sum / (seq_t)m;
    }
    seq_t r = A[n];
    mem_free(D);
    mem_free(A);
    return r;
}
