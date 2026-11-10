/*
 * A323887  Sum of Per Nørgård's "infinity sequence" (A004718) and its
 *          Dirichlet inverse (A323886).
 * https://oeis.org/A323887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A004718(n) + A323886(n), where A323886 is the Dirichlet
 *            inverse of A004718: b(1) = 1, b(n) = -Sum_{d|n, d>1}
 *            A004718(d)*b(n/d); A004718(0)=0, f(2k)=-f(k), f(2k+1)=f(k)+1
 * Range:     i8 n <= 314, i16 n <= 16384, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9286 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    seq_t *f = mem_new(seq_t, N + 1);
    seq_t *b = mem_new(seq_t, N + 1);
    seq_t *acc = mem_new(seq_t, N + 1);
    f[0] = 0;
    for (size_t i = 1; i <= N; i++) {
        if (i & 1) f[i] = f[i >> 1] + 1;
        else f[i] = -f[i >> 1];
    }
    for (size_t m = 1; m <= N; m++) {
        if (m == 1) b[m] = 1;
        else b[m] = -acc[m];
        if (b[m] != 0) {
            for (size_t d = 2; m * d <= N; d++) {
                acc[m * d] += f[d] * b[m];
            }
        }
    }
    seq_t result = f[N] + b[N];
    mem_free(f);
    mem_free(b);
    mem_free(acc);
    return result;
}
