/*
 * A269957  Triangle read by rows, T(n,k) = Sum_{j=k..n}
 *          A269940(n,j)*A269939(j,k), for n>=0 and 0<=k<=n.
 * https://oeis.org/A269957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{j=k..n} A269940(n,j)*A269939(j,k); a(n) = T(r,c)
 *            for the row r and column c of index n in the flattened triangle
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t r = 0, cum = 0;
    while (cum + r + 1 <= n) { cum += r + 1; ++r; }
    idx_t c = n - cum;

    size_t R = (size_t)r;
    size_t N = 2 * R + 1;
    size_t W = R + 1;

    seq_t *st  = mem_new(seq_t, N * N);   /* unsigned Stirling numbers of the first kind */
    seq_t *pas = mem_new(seq_t, N * N);   /* Pascal's triangle */
    seq_t *wd  = mem_new(seq_t, W * W);   /* Ward numbers (A269939) */
    seq_t *A   = mem_new(seq_t, W);       /* row r of A269940 */

    st[0] = (seq_t)1;
    for (size_t i = 0; i < N; ++i) {
        pas[i * N] = (seq_t)1;
        for (size_t j = 1; j <= i; ++j) {
            pas[i * N + j] = pas[(i - 1) * N + j - 1] + (j <= i - 1 ? pas[(i - 1) * N + j] : (seq_t)0);
        }
        if (i > 0) {
            for (size_t j = 1; j <= i; ++j) {
                seq_t t1 = st[(i - 1) * N + j - 1];
                seq_t t2 = (j <= i - 1) ? st[(i - 1) * N + j] * (seq_t)(i - 1) : (seq_t)0;
                st[i * N + j] = t1 + t2;
            }
        }
    }

    /* Ward numbers */
    wd[0] = (seq_t)1;
    for (size_t i = 1; i <= R; ++i) {
        wd[i * W] = (seq_t)0;
        for (size_t k = 1; k <= i; ++k) {
            seq_t t1 = (k <= i - 1) ? (seq_t)k * wd[(i - 1) * W + k] : (seq_t)0;
            seq_t t2 = (seq_t)(i + k - 1) * wd[(i - 1) * W + k - 1];
            wd[i * W + k] = t1 + t2;
        }
    }

    /* A269940(r, j) with separate positive and negative sums */
    for (size_t j = 0; j <= R; ++j) {
        seq_t pos = (seq_t)0, neg = (seq_t)0;
        for (size_t m = 0; m <= j; ++m) {
            seq_t term = pas[(R + j) * N + (R + m)] * st[(R + m) * N + m];
            if ((m + j) & 1U) neg += term; else pos += term;
        }
        A[j] = pos - neg;
    }

    seq_t res = (seq_t)0;
    for (size_t j = (size_t)c; j <= R; ++j)
        res += A[j] * wd[j * W + (size_t)c];

    mem_free(st);
    mem_free(pas);
    mem_free(wd);
    mem_free(A);
    return res;
}
