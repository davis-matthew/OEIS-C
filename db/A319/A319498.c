/*
 * A319498  Number T(n,k) of sets of nonempty words with a total of n letters
 *          over k-ary alphabet such that for k>0 the k-th letter occurs at
 *          least once and within each word every letter of the alphabet is at
 *          least as frequent as the subsequent alphabet letter; triangle
 *          T(n,k), n>=0, 0<=k<=n, read by rows.
 * https://oeis.org/A319498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A292795(n,k) - A292795(n,k-1) for k>0, T(n,0)=0^n;
 *            A292795(n,k)=[x^n] Prod_{m>=1} (1+x^m)^w(m,k), w(m,k)=number of
 *            k-ary words of length m with non-increasing letter counts; the
 *            flattened index is read by rows
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 103, u64 n <= 249,
 *            u128 n <= 658, big any n
 * Validated: standard, pass, 71 terms through n = 9149 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t T319498_coef(const seq_t *w, idx_t r)
{
    size_t N = (size_t)r + 1;
    seq_t *p = mem_new(seq_t, N);
    seq_t *q = mem_new(seq_t, N);
    p[0] = 1;
    for (size_t m = 1; m <= (size_t)r; m++) {
        seq_t ww = w[m];
        if (ww == 0) continue;
        for (size_t i = 0; i < N; i++) q[i] = p[i];
        seq_t c = 1;
        for (size_t t = 1; t * m <= (size_t)r; t++) {
            if ((seq_t)t > ww) break;
            c = c * (ww - (seq_t)t + 1) / (seq_t)t;
            for (size_t i = 0; i + t * m <= (size_t)r; i++)
                q[i + t * m] += p[i] * c;
        }
        seq_t *tmp = p; p = q; q = tmp;
    }
    seq_t res = p[r];
    mem_free(p);
    mem_free(q);
    return res;
}

seq_t a(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    idx_t k = n - r * (r + 1) / 2;
    if (k == 0) return (r == 0) ? 1 : 0;
    size_t N = (size_t)r + 1;
    seq_t *C = mem_new(seq_t, N * N);
    for (size_t i = 0; i < N; i++) {
        C[i * N] = 1;
        for (size_t j = 1; j <= i; j++)
            C[i * N + j] = C[(i - 1) * N + j - 1] + (j <= i - 1 ? C[(i - 1) * N + j] : 0);
    }
    seq_t *F = mem_new(seq_t, N * N);
    seq_t *G = mem_new(seq_t, N * N);
    seq_t *P = mem_new(seq_t, N);
    seq_t *wprev = mem_new(seq_t, N);
    seq_t *wcur = mem_new(seq_t, N);
    F[0] = 1;
    for (idx_t j = 1; j <= k; j++) {
        for (size_t i = 0; i < N * N; i++) G[i] = 0;
        for (size_t s = 0; s < N; s++) {
            seq_t acc = 0;
            for (size_t c = 0; c <= s; c++) {
                acc += F[s * N + c];
                P[c] = acc;
            }
            for (size_t c2 = 0; s + c2 < N; c2++) {
                seq_t pv = P[c2 < s ? c2 : s];
                if (pv == 0) continue;
                G[(s + c2) * N + c2] += pv * C[(s + c2) * N + c2];
            }
        }
        seq_t *tmp = F; F = G; G = tmp;
        if (j >= k - 1) {
            seq_t *dst = (j == k) ? wcur : wprev;
            for (size_t m = 0; m < N; m++) {
                seq_t sum = 0;
                for (size_t c = 0; c <= m; c++) sum += F[m * N + c];
                dst[m] = sum;
            }
        }
    }
    if (k == 1) {
        for (size_t m = 0; m < N; m++) wprev[m] = 0;
    }
    seq_t res = T319498_coef(wcur, r) - T319498_coef(wprev, r);
    mem_free(C);
    mem_free(F);
    mem_free(G);
    mem_free(P);
    mem_free(wprev);
    mem_free(wcur);
    return res;
}
