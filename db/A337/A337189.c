/*
 * A337189  Numbers k such that k divides A340180(k).
 * https://oeis.org/A337189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search over k for those k with k | A340180(k), where A340180(k)
 *            = sigma(k)*phi(k) - Sum_{d|rad(k)} mu(d)*d*G(floor(sigma(k)/d),
 *            floor((k-1)/d)) with G(N,L) = Sum_{y=1..L} y*floor(N/y) =
 *            Sum_{m<=N} sigma(m) - Sum_{y=L+1..N} y*floor(N/y), using a
 *            sieved prefix sum of sigma; a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint64_t *A337189_P = NULL;   /* prefix sums of sigma up to M */
static uint32_t *A337189_spf = NULL; /* smallest prime factor up to K */
static uint64_t A337189_K = 0, A337189_M = 0;

static void A337189_build(uint64_t newK) {
    free(A337189_P);
    free(A337189_spf);
    A337189_K = newK;
    A337189_M = 5 * newK;
    uint64_t M = A337189_M, K = A337189_K;
    A337189_P = (uint64_t *)calloc((size_t)(M + 1), sizeof(uint64_t));
    for (uint64_t d = 1; d <= M; d++)
        for (uint64_t j = d; j <= M; j += d)
            A337189_P[j] += d;
    for (uint64_t i = 1; i <= M; i++) A337189_P[i] += A337189_P[i - 1];
    A337189_spf = (uint32_t *)calloc((size_t)(K + 1), sizeof(uint32_t));
    for (uint64_t i = 2; i <= K; i++)
        if (A337189_spf[i] == 0)
            for (uint64_t j = i; j <= K; j += i)
                if (A337189_spf[j] == 0) A337189_spf[j] = (uint32_t)i;
}

static uint64_t A337189_T(uint64_t h) { return h * (h + 1) / 2; }

/* Sum_{y<=N} y*floor(N/y) = Sum_{m<=N} sigma(m) */
static uint64_t A337189_Ssig(uint64_t N) {
    if (N <= A337189_M) return A337189_P[N];
    uint64_t s = 0, y = 1;
    while (y <= N) {
        uint64_t q = N / y, hi = N / q;
        s += q * (A337189_T(hi) - A337189_T(y - 1));
        y = hi + 1;
    }
    return s;
}

/* Sum_{y=1..L} y*floor(N/y) */
static uint64_t A337189_G(uint64_t N, uint64_t L) {
    uint64_t s = A337189_Ssig(N);
    if (L >= N) return s;
    uint64_t y = L + 1;
    while (y <= N) {
        uint64_t q = N / y, hi = N / q;
        s -= q * (A337189_T(hi) - A337189_T(y - 1));
        y = hi + 1;
    }
    return s;
}

static int A337189_check(uint64_t k) {
    if (k == 1) return 1;
    uint64_t sigma = A337189_P[k] - A337189_P[k - 1];
    uint64_t ps[20];
    int np = 0;
    uint64_t t = k, phi = k;
    while (t > 1) {
        uint64_t p = A337189_spf[t];
        while (t % p == 0) t /= p;
        phi = phi / p * (p - 1);
        ps[np++] = p;
    }
    uint64_t dv[1 << 10];
    int sg[1 << 10];
    size_t nd = 1;
    dv[0] = 1; sg[0] = 1;
    for (int i = 0; i < np; i++) {
        size_t cur = nd;
        for (size_t j = 0; j < cur; j++) {
            dv[nd] = dv[j] * ps[i];
            sg[nd] = -sg[j];
            nd++;
        }
    }
    uint64_t S = 0;
    for (size_t j = 0; j < nd; j++) {
        uint64_t d = dv[j];
        uint64_t term = d * A337189_G(sigma / d, (k - 1) / d);
        if (sg[j] > 0) S += term; else S -= term;
    }
    uint64_t total = sigma * phi - S;
    return (total % k) == 0;
}

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t next_k = 1;
    while (cnt < (size_t)n) {
        if (next_k > A337189_K) {
            uint64_t nk = A337189_K ? A337189_K * 2 : ((uint64_t)1 << 16);
            A337189_build(nk);
        }
        if (A337189_check(next_k)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 64;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = next_k;
        }
        next_k++;
    }
    return (seq_t)terms[n - 1];
}
