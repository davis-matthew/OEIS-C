/*
 * A368695  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A368694(i) = A368694(j) for all i, j >= 0.
 * https://oeis.org/A368695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = restricted growth transform of A368694(A163511(n)), where
 *            A368694 is the Dirichlet inverse of 2^(v2(sigma(n))); computed
 *            by divisor-lattice recursion g(m) = -Sum_{d|m, d>1} f(d)*g(m/d),
 *            g(1) = 1, f(d) = 2^v2(sigma(d)); g is tracked mod 2^64 and mod
 *            2^61-1 for the equality test
 * Range:     u8 n <= 1045, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 105 terms through n = 9983 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const uint32_t A368695_pr[20] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};

static const uint64_t A368695_M2 = (((uint64_t)1) << 61) - 1;

static uint64_t A368695_mm(uint64_t a, uint64_t b)
{
    unsigned __int128 t = (unsigned __int128)a * b;
    return (uint64_t)(t % A368695_M2);
}

static int A368695_v2(uint64_t x)
{
    int c = 0;
    while (x && !(x & 1)) { x >>= 1; c++; }
    return c;
}

static void A368695_key(idx_t n, uint64_t out[2])
{
    int e[24];
    int k = 0, i;
    idx_t pos[64];
    int cnt = 0;
    idx_t bitpos = 1, tmp = n, prev = 0;
    while (tmp) {
        if (tmp & 1) pos[cnt++] = bitpos;
        bitpos++;
        tmp >>= 1;
    }
    if (cnt == 0) { out[0] = 1; out[1] = 1; return; }
    k = cnt;
    for (i = 0; i < k; i++) {
        e[i] = (int)(pos[i] - prev - 1);
        prev = pos[i];
    }
    e[k - 1]++;
    size_t D = 1;
    size_t w[24];
    for (i = 0; i < k; i++) { w[i] = D; D *= (size_t)(e[i] + 1); }
    uint8_t *dig = (uint8_t *)malloc(D * k);
    uint64_t *F1 = (uint64_t *)malloc(D * sizeof(uint64_t));
    uint64_t *F2 = (uint64_t *)malloc(D * sizeof(uint64_t));
    uint64_t *G1 = (uint64_t *)malloc(D * sizeof(uint64_t));
    uint64_t *G2 = (uint64_t *)malloc(D * sizeof(uint64_t));
    size_t t, s;
    for (t = 0; t < D; t++) {
        int v = 0;
        for (i = 0; i < k; i++) {
            int d = (int)((t / w[i]) % (size_t)(e[i] + 1));
            dig[t * k + i] = (uint8_t)d;
            if (i > 0 && (d & 1)) {
                v += A368695_v2(A368695_pr[i] + 1) + A368695_v2((uint64_t)d + 1) - 1;
            }
        }
        F1[t] = (v >= 64) ? 0 : (((uint64_t)1) << v);
        uint64_t f2 = 1;
        for (i = 0; i < v; i++) { f2 <<= 1; if (f2 >= A368695_M2) f2 -= A368695_M2; }
        F2[t] = f2;
    }
    G1[0] = 1; G2[0] = 1;
    for (t = 1; t < D; t++) {
        uint64_t s1 = 0, s2 = 0;
        for (s = 1; s <= t; s++) {
            int ok = 1;
            for (i = 0; i < k; i++) {
                if (dig[s * k + i] > dig[t * k + i]) { ok = 0; break; }
            }
            if (!ok) continue;
            s1 += F1[s] * G1[t - s];
            s2 += A368695_mm(F2[s], G2[t - s]);
            if (s2 >= A368695_M2) s2 -= A368695_M2;
        }
        G1[t] = (uint64_t)0 - s1;
        G2[t] = s2 ? (A368695_M2 - s2) : 0;
    }
    out[0] = G1[D - 1];
    out[1] = G2[D - 1];
    free(dig); free(F1); free(F2); free(G1); free(G2);
}

static uint64_t *A368695_dk = 0;   /* distinct keys, pairs */
static size_t A368695_nd = 0, A368695_cap = 0;
static uint64_t *A368695_cls = 0;  /* class of each computed index */
static size_t A368695_ncls = 0, A368695_clscap = 0;

seq_t a(idx_t n)
{
    while (A368695_ncls <= (size_t)n) {
        uint64_t key[2];
        size_t j;
        A368695_key((idx_t)A368695_ncls, key);
        for (j = 0; j < A368695_nd; j++) {
            if (A368695_dk[2 * j] == key[0] && A368695_dk[2 * j + 1] == key[1]) break;
        }
        if (j == A368695_nd) {
            if (A368695_nd == A368695_cap) {
                A368695_cap = A368695_cap ? A368695_cap * 2 : 1024;
                A368695_dk = (uint64_t *)realloc(A368695_dk, A368695_cap * 2 * sizeof(uint64_t));
            }
            A368695_dk[2 * j] = key[0];
            A368695_dk[2 * j + 1] = key[1];
            A368695_nd++;
        }
        if (A368695_ncls == A368695_clscap) {
            A368695_clscap = A368695_clscap ? A368695_clscap * 2 : 1024;
            A368695_cls = (uint64_t *)realloc(A368695_cls, A368695_clscap * sizeof(uint64_t));
        }
        A368695_cls[A368695_ncls++] = (uint64_t)j + 1;
    }
    return (seq_t)A368695_cls[(size_t)n];
}
