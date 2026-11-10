/*
 * A261332  Expansion of Product_{k>=1} (1+x^k)^(A002203(k)).
 * https://oeis.org/A261332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] Product_{k>=1} (1+x^k)^A002203(k), where A002203(k)
 *            = 2*A002203(k-1) + A002203(k-2), A002203(0) = A002203(1) = 2;
 *            each factor is expanded as Sum_j binomial(A002203(k), j)*x^(j*k)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 85,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 848 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    size_t N = (size_t)n;
    seq_t *c = mem_new(seq_t, N + 1);
    seq_t *f = mem_new(seq_t, N + 1);
    seq_t *b = mem_new(seq_t, N + 1);
    c[0] = 2;
    c[1] = 2;
    for (size_t i = 2; i <= N; i++) c[i] = 2 * c[i - 1] + c[i - 2];
    f[0] = 1;
    for (size_t k = 1; k <= N; k++) {
        seq_t e = c[k];
        size_t jmax = N / k;
        size_t J = 0;
        b[0] = 1;
        for (size_t j = 1; j <= jmax; j++) {
            if ((seq_t)j > e) break;
            b[j] = b[j - 1] * (e - (seq_t)(j - 1)) / (seq_t)j;
            J = j;
        }
        if (J == 0) continue;
        for (size_t m = N; m >= k; m--) {
            seq_t add = 0;
            for (size_t j = 1; j <= J && j * k <= m; j++) {
                add += b[j] * f[m - j * k];
            }
            f[m] += add;
            if (m == k) break;
        }
    }
    seq_t r = f[N];
    mem_free(c);
    mem_free(f);
    mem_free(b);
    return r;
}
