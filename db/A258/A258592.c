/*
 * A258592  Values of k such that the number of rooted trees with k nodes
 *          (A000081(k)) is even.
 * https://oeis.org/A258592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th k >= 0 with A000081(k) even, where A000081
 *            is computed mod 2^64 from A000081(k+1) = [x^k] Product_{j>=1}
 *            (1-x^j)^(-A000081(j)), A000081(0) = 0, A000081(1) = 1. Each
 *            binomial(c+j-1, j) is computed exactly mod 2^64 by keeping its
 *            odd part and its power of 2 separately. A differe...
 * Range:     u8 n <= 140, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 81 terms through n = 4628 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint64_t *A258592_T = NULL;   /* A000081(k) mod 2^64, k = 0..N */
static size_t A258592_N = 0;
static size_t *A258592_ev = NULL;    /* k with A000081(k) even */
static size_t A258592_evn = 0;

static int a258592_ctz(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctzll(x);
#else
    int r = 0;
    while (!(x & 1)) { x >>= 1; r++; }
    return r;
#endif
}

static void a258592_build(size_t N)
{
    size_t L = N, i, k, m, j;
    const uint64_t MASK = (((uint64_t)1) << 63) - 1;
    uint64_t *T = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    uint64_t *P = (uint64_t *)calloc(L + 2, sizeof(uint64_t));
    uint64_t *M = (uint64_t *)calloc(L + 2, sizeof(uint64_t));
    uint64_t *INV = (uint64_t *)calloc(L + 2, sizeof(uint64_t));
    unsigned char *V2 = (unsigned char *)calloc(L + 2, 1);

    for (i = 1; i <= L; i++) {
        int s = a258592_ctz((uint64_t)i);
        uint64_t o = ((uint64_t)i) >> s, inv = o;
        int it;
        for (it = 0; it < 6; it++) inv *= 2 - o * inv;
        INV[i] = inv;
        V2[i] = (unsigned char)s;
    }

    P[0] = 1;
    T[0] = 0;
    T[1] = 1;
    for (k = 1; k < N; k++) {
        uint64_t c = T[k] & MASK;
        size_t J = (L - 1) / k;
        M[0] = 1;
        if (c == 0) {
            for (j = 1; j <= J; j++) M[j] = 0;
        } else {
            uint64_t U = 1;
            long E = 0;
            for (j = 1; j <= J; j++) {
                uint64_t x = c + (uint64_t)(j - 1);
                int t = a258592_ctz(x);
                U *= x >> t;
                E += t;
                U *= INV[j];
                E -= V2[j];
                M[j] = (E >= 64) ? 0 : (U << E);
            }
        }
        if (L >= 1 && L - 1 >= k) {
            for (m = L - 1; m >= k; m--) {
                uint64_t sum = P[m];
                size_t r = m - k;
                for (j = 1;; j++) {
                    sum += M[j] * P[r];
                    if (r < k) break;
                    r -= k;
                }
                P[m] = sum;
            }
        }
        T[k + 1] = P[k];
    }

    free(P); free(M); free(INV); free(V2);
    free(A258592_T);
    A258592_T = T;
    A258592_N = N;

    free(A258592_ev);
    A258592_ev = (size_t *)malloc((N + 2) * sizeof(size_t));
    A258592_evn = 0;
    for (k = 0; k <= N; k++)
        if ((T[k] & 1) == 0) A258592_ev[A258592_evn++] = k;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A258592_evn < (size_t)n) {
        size_t N = A258592_N ? A258592_N * 2 : 64;
        while (N < 2 * (size_t)n + 16) N *= 2;
        a258592_build(N);
    }
    return (seq_t)A258592_ev[(size_t)n - 1];
}
