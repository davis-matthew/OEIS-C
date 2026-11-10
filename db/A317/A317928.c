/*
 * A317928  Denominators of rational valued sequence whose Dirichlet
 *          convolution with itself yields A005187.
 * https://oeis.org/A317928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of f(n), f(1) = 1, f(n) = (1/2)*(A005187(n) -
 *            Sum_{d|n, 1<d<n} f(d)*f(n/d)); each f(n) is stored exactly as
 *            +-m/2^e
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9358 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t A005187_a(idx_t n) {
    seq_t result = 0;
    seq_t power = (seq_t)n;
    while (power > 0) {
        result += power;
        power >>= 1;
    }
    return result;
}

static seq_t *f317928_m = 0;
static int *f317928_e = 0;
static bool *f317928_neg = 0;
static idx_t f317928_cap = 0;

static void f317928_build(idx_t cap) {
    idx_t n, d;
    if (f317928_m) { mem_free(f317928_m); mem_free(f317928_e); mem_free(f317928_neg); }
    f317928_m = mem_new(seq_t, (size_t)cap + 1);
    f317928_e = mem_new(int, (size_t)cap + 1);
    f317928_neg = mem_new(bool, (size_t)cap + 1);
    f317928_cap = cap;
    f317928_m[1] = 1; f317928_e[1] = 0; f317928_neg[1] = false;
    for (n = 2; n <= cap; n++) {
        int E = 0;
        for (d = 2; d * d <= n; d++) {
            if (n % d == 0) {
                int e = f317928_e[d] + f317928_e[n / d];
                if (e > E) E = e;
            }
        }
        seq_t pos = A005187_a(n) << E;
        seq_t neg = 0;
        for (d = 2; d * d <= n; d++) {
            if (n % d == 0) {
                idx_t q = n / d;
                int e = f317928_e[d] + f317928_e[q];
                seq_t t = (f317928_m[d] * f317928_m[q]) << (E - e);
                if (d != q) t = t * 2;
                if (f317928_neg[d] != f317928_neg[q]) pos += t;
                else neg += t;
            }
        }
        seq_t mag;
        bool sg;
        if (pos >= neg) { mag = pos - neg; sg = false; }
        else { mag = neg - pos; sg = true; }
        int e = E + 1;
        if (mag == 0) { e = 0; sg = false; }
        while (e > 0 && (mag & 1) == 0) { mag >>= 1; e--; }
        f317928_m[n] = mag; f317928_e[n] = e; f317928_neg[n] = sg;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 1;
    if (n > f317928_cap) {
        idx_t cap = f317928_cap * 2;
        if (cap < n) cap = n;
        if (cap < 64) cap = 64;
        f317928_build(cap);
    }
    return ((seq_t)1) << f317928_e[n];
}
