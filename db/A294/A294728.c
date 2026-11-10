/*
 * A294728  Number of Q graphs with 2*n vertices symmetrical about a
 *          distinguished edge.
 * https://oeis.org/A294728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i+2k=n-4} c(i)*d(k), a(n)=0 for n<4, where c(i) =
 *            A007165(i+1) (c(0)=1, c(m)=Sum_{i+j=m-1}(-1)^j c(i)c(j) for odd
 *            m, c(m)=c(m-1)+Sum_{i odd,i+j=m-1}c(i)c(j) for even m), d(0)=1,
 *            d(k)=A003168(k+1) for k>=1 (equivalent to G.f. H(x)*(B(x^2)-x^2)
 *            shifted by x^4)
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* A003168(0..N) via P = 1 + z*(2*P^3 - P^2), division-free */
static void a294728_blobs(idx_t N, seq_t *out) {
    seq_t *A = mem_new(seq_t, (size_t)(N + 1));
    seq_t *Q = mem_new(seq_t, (size_t)(N + 1));
    seq_t *R = mem_new(seq_t, (size_t)(N + 1));
    A[0] = 1; Q[0] = 1; R[0] = 1;
    out[0] = 1;
    for (idx_t m = 1; m <= N; m++) {
        A[m] = R[m - 1] * 2 - Q[m - 1];
        out[m] = A[m];
        if (m == N) break;
        seq_t q = 0;
        for (idx_t i = 0; i <= m; i++) q += A[i] * A[m - i];
        Q[m] = q;
        seq_t r = 0;
        for (idx_t i = 0; i <= m; i++) r += Q[i] * A[m - i];
        R[m] = r;
    }
    mem_free(A); mem_free(Q); mem_free(R);
}

seq_t a(idx_t n) {
    if (n < 4) return 0;
    idx_t m = n - 4;
    seq_t *c = mem_new(seq_t, (size_t)(m + 1));
    c[0] = 1;
    for (idx_t t = 1; t <= m; t++) {
        idx_t s = t - 1;
        if (t & 1) {
            seq_t pos = 0, neg = 0;
            for (idx_t i = 0; i <= s; i++) {
                seq_t p = c[i] * c[s - i];
                if (i & 1) neg += p; else pos += p;
            }
            c[t] = pos - neg;
        } else {
            seq_t v = c[t - 1];
            for (idx_t i = 1; i <= s; i += 2) v += c[i] * c[s - i];
            c[t] = v;
        }
    }
    idx_t K = m / 2;
    seq_t *B = mem_new(seq_t, (size_t)(K + 2));
    a294728_blobs(K + 1, B);
    seq_t res = 0;
    for (idx_t k = 0; k <= K; k++) {
        seq_t d = (k == 0) ? (seq_t)1 : B[k + 1];
        res += c[m - 2 * k] * d;
    }
    mem_free(c); mem_free(B);
    return res;
}
