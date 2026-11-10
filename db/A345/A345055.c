/*
 * A345055  Dirichlet inverse of A011772.
 * https://oeis.org/A345055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{d|n, d<n} A011772(n/d)*a(d), a(1) = 1 (Dirichlet
 *            inverse of A011772; computed over the divisors of n)
 * Range:     i8 n <= 41, i16 n <= 1319, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 91 terms through n = 9003 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static idx_t A011772_f(idx_t n)
{
    if (n <= 1) return 1;
    idx_t lim = n * 2u;
    for (idx_t m = 1; m < lim; ++m) {
        unsigned __int128 t = (unsigned __int128)m * (m + 1) / 2;
        if ((t % n) == 0) return m;
    }
    return lim - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t cap = 64, cnt = 0;
    idx_t *dv = (idx_t *)malloc(cap * sizeof(idx_t));
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (cnt + 2 > cap) {
                cap *= 2;
                dv = (idx_t *)realloc(dv, cap * sizeof(idx_t));
            }
            dv[cnt++] = i;
            if (i != n / i) dv[cnt++] = n / i;
        }
    }
    for (size_t i = 1; i < cnt; ++i) {
        idx_t v = dv[i];
        size_t j = i;
        while (j > 0 && dv[j - 1] > v) { dv[j] = dv[j - 1]; --j; }
        dv[j] = v;
    }
    idx_t *fv = (idx_t *)malloc(cnt * sizeof(idx_t));
    for (size_t i = 0; i < cnt; ++i) fv[i] = A011772_f(dv[i]);
    seq_t *b = mem_new(seq_t, cnt);
    b[0] = 1;
    for (size_t i = 1; i < cnt; ++i) {
        seq_t s = 0;
        for (size_t j = 0; j < i; ++j) {
            if (dv[i] % dv[j] == 0) {
                idx_t q = dv[i] / dv[j];
                size_t k = 0;
                while (dv[k] != q) ++k;
                s += (seq_t)fv[k] * b[j];
            }
        }
        b[i] = -s;
    }
    seq_t r = b[cnt - 1];
    mem_free(b);
    free(fv);
    free(dv);
    return r;
}
