/*
 * A294724  Number of Q graphs with 2*n vertices rooted at an internal edge.
 * https://oeis.org/A294724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n+1)] P(x)*D(x)^2, where P(x) = Sum_{k>=1} A003169(k)
 *            x^k and D(x) = Sum_{k>=2} A003168(k-1) x^k (the OEIS g.f.
 *            P(x)*(B(x)-x)^2 with B shifted by one power of x), i.e.
 *            a(1)=a(2)=a(3)=0, a(4)=1. Both series are computed
 *            division-free: A003168: B = 1 + z*(2B^3 - B^2); A003169: A = x +
 *            x*...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A294724_res = NULL;
static idx_t A294724_done = 0;

static void A294724_build(idx_t n) {
    idx_t N = n + 1;
    idx_t m, i, j;
    seq_t *B = mem_new(seq_t, (size_t)(N + 2));
    seq_t *Q = mem_new(seq_t, (size_t)(N + 2));
    seq_t *R = mem_new(seq_t, (size_t)(N + 2));
    seq_t *P = mem_new(seq_t, (size_t)(N + 2));
    seq_t *S2 = mem_new(seq_t, (size_t)(N + 2));
    seq_t *D = mem_new(seq_t, (size_t)(N + 2));
    seq_t *E = mem_new(seq_t, (size_t)(N + 2));
    seq_t *res = mem_new(seq_t, (size_t)(N + 2));

    /* A003168 coefficients B[0..N] */
    B[0] = 1; Q[0] = 1; R[0] = 1;
    for (m = 1; m <= N; m++) {
        B[m] = R[m - 1] * 2 - Q[m - 1];
        {
            seq_t q = 0;
            for (i = 0; i <= m; i++) q += B[i] * B[m - i];
            Q[m] = q;
        }
        {
            seq_t r = 0;
            for (i = 0; i <= m; i++) r += Q[i] * B[m - i];
            R[m] = r;
        }
    }
    /* A003169 coefficients P[1..N] */
    P[0] = 0; S2[0] = 0;
    for (m = 1; m <= N; m++) {
        seq_t s2 = 0, s3 = 0;
        for (i = 1; i < m; i++) s2 += P[i] * P[m - i];
        S2[m] = s2;
        for (i = 1; i + 2 <= m; i++) s3 += P[i] * S2[m - i];
        P[m] = (seq_t)(m == 1 ? 1 : 0) + P[m - 1] + s2 * 2 - s3;
    }
    /* D[k] = A003168(k-1), k>=2 */
    D[0] = 0;
    if (N >= 1) D[1] = 0;
    for (m = 2; m <= N; m++) D[m] = B[m - 1];
    for (m = 0; m <= N; m++) {
        seq_t e = 0;
        for (j = 0; j <= m; j++) e += D[j] * D[m - j];
        E[m] = e;
    }
    res[0] = 0;
    for (m = 1; m <= n; m++) {
        seq_t s = 0;
        idx_t t = m + 1;
        for (i = 1; i <= t; i++) s += P[i] * E[t - i];
        res[m] = s;
    }
    if (A294724_res) mem_free(A294724_res);
    A294724_res = res;
    A294724_done = n;
    mem_free(B); mem_free(Q); mem_free(R); mem_free(P);
    mem_free(S2); mem_free(D); mem_free(E);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n > A294724_done) {
        idx_t target = n;
        if (target < 2 * A294724_done && A294724_done < 5000) target = 2 * A294724_done;
        if (target < 64) target = 64;
        A294724_build(target);
    }
    return A294724_res[n];
}
