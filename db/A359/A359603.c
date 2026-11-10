/*
 * A359603  Dirichlet inverse of function f(n) = 1+(A003415(n)*A276086(n)),
 *          where A003415 is the arithmetic derivative and A276086 is the
 *          primorial base exp-function.
 * https://oeis.org/A359603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = 1; a(n) = -Sum_{d|n, d<n} f(n/d)*a(d) for n > 1, where
 *            f(m) = 1 + A003415(m)*A276086(m) (Dirichlet inverse of f),
 *            computed with a divisor sieve
 * Range:     i8 n <= 8, i16 n <= 26, i32 n <= 171, i64 n <= 1677,
 *            i128 n <= 11550, big any n
 * Validated: standard, pass, 64 terms through n = 9845 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* A003415: arithmetic derivative of m */
static seq_t A003415_d(idx_t m) {
    if (m < 2) return 0;
    seq_t res = 0;
    idx_t rem = m;
    for (idx_t p = 2; p * p <= rem; p++) {
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) { rem /= p; e++; }
            res += (seq_t)(m / p) * (seq_t)e;
        }
    }
    if (rem > 1) res += (seq_t)(m / rem);
    return res;
}

/* A276086: primorial base exp-function */
static seq_t A276086_e(idx_t m) {
    static const unsigned pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    seq_t res = 1;
    idx_t r = m;
    for (int k = 0; r > 0 && k < 16; k++) {
        idx_t d = r % pr[k];
        r /= pr[k];
        for (idx_t j = 0; j < d; j++) res *= (seq_t)pr[k];
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    size_t N = (size_t)n;
    seq_t *f = mem_new(seq_t, N + 1);
    seq_t *acc = mem_new(seq_t, N + 1);
    seq_t *r = mem_new(seq_t, N + 1);
    for (size_t m = 1; m <= N; m++) {
        f[m] = 1 + A003415_d((idx_t)m) * A276086_e((idx_t)m);
    }
    for (size_t d = 1; d <= N; d++) {
        if (d == 1) r[d] = 1;
        else r[d] = -acc[d];
        for (size_t m = 2 * d; m <= N; m += d) {
            acc[m] += f[m / d] * r[d];
        }
    }
    seq_t result = r[N];
    mem_free(f);
    mem_free(acc);
    mem_free(r);
    return result;
}
