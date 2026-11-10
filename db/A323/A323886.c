/*
 * A323886  Dirichlet inverse of A004718, Per Nørgård's "infinity sequence".
 * https://oeis.org/A323886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Dirichlet inverse of A004718 (f): b(1)=1/f(1)=1, b(n) =
 *            -Sum_{d|n, d>1} f(d)*b(n/d); f(0)=0, f(2n)=-f(n), f(2n+1)=f(n)+1
 * Range:     i8 n <= 314, i16 n <= 16384, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 120 terms through n = 9356 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A323886_tab = 0;
static idx_t A323886_size = 0;

static void A323886_build(idx_t N) {
    seq_t *f = mem_new(seq_t, (size_t)N + 1);
    seq_t *acc = mem_new(seq_t, (size_t)N + 1);
    seq_t *b = mem_new(seq_t, (size_t)N + 1);
    idx_t m, j;
    f[0] = 0;
    for (m = 1; m <= N; m++) {
        if (m & 1) f[m] = f[m >> 1] + 1;
        else f[m] = -f[m >> 1];
    }
    for (m = 1; m <= N; m++) {
        if (m == 1) b[m] = 1;
        else b[m] = -acc[m];
        for (j = 2; m * j <= N; j++) {
            acc[m * j] += f[j] * b[m];
        }
    }
    mem_free(f);
    mem_free(acc);
    if (A323886_tab) mem_free(A323886_tab);
    A323886_tab = b;
    A323886_size = N;
}

seq_t a(idx_t n) {
    if (n > A323886_size) {
        idx_t N = A323886_size * 2;
        if (N < n) N = n;
        if (N < 1024) N = 1024;
        A323886_build(N);
    }
    return A323886_tab[n];
}
