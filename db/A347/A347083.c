/*
 * A347083  Sum of -A168036 and its Dirichlet inverse.
 * https://oeis.org/A347083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   with h(n) = n - n' (= -A168036(n)) and g its Dirichlet inverse:
 *            g(1)=1, g(n) = -Sum_{d|n, d>1} h(d)*g(n/d); a(n) = g(n) + h(n)
 *            (so a(1)=2)
 * Range:     i8 n <= 84, i16 n <= 4095, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 98 terms through n = 7950 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A168036_arith_deriv(idx_t n)
{
    if (n <= 1) return 0;
    seq_t nseq = (seq_t)n;
    seq_t deriv = 0;
    idx_t tmp = n;
    if (tmp % 2 == 0) {
        int e = 0;
        while (tmp % 2 == 0) { tmp /= 2; e++; }
        deriv += (nseq / 2) * e;
    }
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; e++; }
            deriv += (nseq / p) * e;
        }
    }
    if (tmp > 1) deriv += (nseq / tmp);
    return deriv;
}

static seq_t A347083_h(idx_t d)
{
    return (seq_t)d - A168036_arith_deriv(d);
}

seq_t a(idx_t n)
{
    if (n <= 1) return 2;
    size_t cap = 64, lo = 0, hi = 0;
    idx_t *small = (idx_t *)malloc(cap * sizeof(idx_t));
    idx_t *large = (idx_t *)malloc(cap * sizeof(idx_t));
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            if (lo >= cap) {
                cap *= 2;
                small = (idx_t *)realloc(small, cap * sizeof(idx_t));
                large = (idx_t *)realloc(large, cap * sizeof(idx_t));
            }
            small[lo++] = i;
            if (i != n / i) large[hi++] = n / i;
        }
    }
    size_t D = lo + hi;
    idx_t *d = (idx_t *)malloc(D * sizeof(idx_t));
    for (size_t i = 0; i < lo; i++) d[i] = small[i];
    for (size_t i = 0; i < hi; i++) d[lo + i] = large[hi - 1 - i];
    free(small);
    free(large);

    seq_t *h = mem_new(seq_t, D);
    seq_t *g = mem_new(seq_t, D);
    for (size_t i = 0; i < D; i++) h[i] = A347083_h(d[i]);
    g[0] = 1;
    for (size_t i = 1; i < D; i++) {
        seq_t s = 0;
        for (size_t j = 1; j <= i; j++) {
            if (d[i] % d[j] == 0) {
                idx_t q = d[i] / d[j];
                size_t L = 0, R = i;
                while (L < R) {
                    size_t m = (L + R) / 2;
                    if (d[m] < q) L = m + 1; else R = m;
                }
                s += h[j] * g[L];
            }
        }
        g[i] = -s;
    }
    seq_t res = g[D - 1] + h[D - 1];
    mem_free(h);
    mem_free(g);
    free(d);
    return res;
}
