/*
 * A347087  Sum of n and the Dirichlet inverse of n - A003415(n).
 * https://oeis.org/A347087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n + A347082(n), where A347082 is the Dirichlet inverse g
 *            of f(m) = m - A003415(m): g(1)=1, g(n) = -Sum_{d|n, d>1}
 *            f(d)*g(n/d)
 * Range:     i8 n <= 63, i16 n <= 4095, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 100 terms through n = 9687 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t A003415_a(idx_t n) {
    if (n == 0 || n == 1)
        return 0;
    seq_t result = 0;
    seq_t original_n = (seq_t)n;
    seq_t remaining = original_n;
    for (seq_t p = 2; p * p <= remaining; p++) {
        int exponent = 0;
        while (remaining % p == 0) {
            exponent++;
            remaining /= p;
        }
        if (exponent > 0) {
            result += (original_n / p) * (seq_t)exponent;
        }
    }
    if (remaining > 1) {
        result += original_n / remaining;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* collect divisors */
    size_t cap = 16, cnt = 0;
    idx_t *dv = mem_new(idx_t, cap);
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            if (cnt + 2 > cap) {
                cap *= 2;
                dv = mem_resize(idx_t, dv, cap);
            }
            dv[cnt++] = i;
            if (i != n / i) dv[cnt++] = n / i;
        }
    }
    /* insertion sort */
    for (size_t i = 1; i < cnt; i++) {
        idx_t x = dv[i];
        size_t j = i;
        while (j > 0 && dv[j - 1] > x) {
            dv[j] = dv[j - 1];
            j--;
        }
        dv[j] = x;
    }
    seq_t *F = mem_new(seq_t, cnt);
    seq_t *g = mem_new(seq_t, cnt);
    for (size_t i = 0; i < cnt; i++)
        F[i] = (seq_t)dv[i] - A003415_a(dv[i]);
    g[0] = 1;
    for (size_t i = 1; i < cnt; i++) {
        seq_t s = 0;
        for (size_t j = 1; j <= i; j++) {
            if (dv[i] % dv[j] == 0) {
                idx_t q = dv[i] / dv[j];
                size_t k = 0;
                while (dv[k] != q) k++;
                s += F[j] * g[k];
            }
        }
        g[i] = -s;
    }
    seq_t res = (seq_t)n + g[cnt - 1];
    mem_free(dv);
    mem_free(F);
    mem_free(g);
    return res;
}
