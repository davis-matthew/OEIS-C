/*
 * A336641  Numbers k such that A007913(k) divides sigma(k) and A008833(k)-1
 *          either divides A326127(k) (= sigma(k)-core(k)-k), or both are
 *          zero.
 * https://oeis.org/A336641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. Write k = c*s^2, where c = A007913(k)
 *            (squarefree) and s^2 = A008833(k). For each s and each set T of
 *            primes of s that also divide c, let c' = c/prod(T). Every prime
 *            of c' must divide sigma(s^2*prod(T)) * prod_{q|c'}(q+1), so c'
 *            only uses primes from a finite closure. E...
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A336641_spf = NULL;
static uint32_t *A336641_pr = NULL;
static size_t A336641_npr = 0;
static uint64_t A336641_slim = 0;

static uint64_t *A336641_terms = NULL;
static size_t A336641_cnt = 0, A336641_cap = 0;
static uint64_t A336641_X = 0;

static uint64_t A336641_cand[2048];
static int A336641_nc = 0;
static uint64_t A336641_S, A336641_prodT, A336641_N, A336641_Lp, A336641_s;

static uint64_t A336641_isqrt(uint64_t x) {
    uint64_t r = 0, b;
    for (b = (uint64_t)1 << 31; b; b >>= 1) {
        uint64_t t = r | b;
        if (t <= x / t) r = t;
    }
    return r;
}

static void A336641_sieve(uint64_t lim) {
    uint64_t i, j;
    size_t c = 0;
    if (lim <= A336641_slim) return;
    free(A336641_spf);
    free(A336641_pr);
    A336641_spf = (uint32_t *)calloc(lim + 1, sizeof(uint32_t));
    A336641_pr = (uint32_t *)malloc(sizeof(uint32_t) * (lim / 2 + 16));
    for (i = 2; i <= lim; i++) {
        if (A336641_spf[i] == 0) {
            A336641_spf[i] = (uint32_t)i;
            A336641_pr[c++] = (uint32_t)i;
            for (j = i * i; j <= lim; j += i)
                if (A336641_spf[j] == 0) A336641_spf[j] = (uint32_t)i;
        }
    }
    A336641_npr = c;
    A336641_slim = lim;
}

/* distinct prime factors of R that are <= L */
static int A336641_pfac(uint64_t R, uint64_t L, uint64_t *out) {
    int c = 0;
    size_t i;
    for (i = 0; i < A336641_npr; i++) {
        uint64_t p = A336641_pr[i];
        if (p > L) return c;
        if (p * p > R) break;
        if (R % p == 0) {
            out[c++] = p;
            do R /= p; while (R % p == 0);
        }
    }
    if (R > 1 && R <= L) out[c++] = R;
    return c;
}

static void A336641_push(uint64_t k) {
    if (A336641_cnt == A336641_cap) {
        A336641_cap = A336641_cap ? A336641_cap * 2 : 64;
        A336641_terms = (uint64_t *)realloc(A336641_terms, A336641_cap * sizeof(uint64_t));
    }
    A336641_terms[A336641_cnt++] = k;
}

static void A336641_addcand(uint64_t q) {
    int i;
    if (q < 2 || q > A336641_Lp) return;
    if (A336641_s % q == 0) return;
    for (i = 0; i < A336641_nc; i++)
        if (A336641_cand[i] == q) return;
    if (A336641_nc < 2048) A336641_cand[A336641_nc++] = q;
}

static void A336641_dfs(int i, uint64_t cp, uint64_t sp) {
    uint64_t c = A336641_prodT * cp, k = A336641_S * c, sg = A336641_N * sp;
    int j;
    if (sg % c == 0) {
        if (A336641_S == 1) {
            if (sg == 2 * k) A336641_push(k);
        } else {
            uint64_t r = sg - c - k;
            if (r % (A336641_S - 1) == 0) A336641_push(k);
        }
    }
    for (j = i; j < A336641_nc; j++) {
        uint64_t q = A336641_cand[j];
        if (q > A336641_Lp / cp) break;
        A336641_dfs(j + 1, cp * q, sp * (q + 1));
    }
}

static int A336641_cmp(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void A336641_run(uint64_t X) {
    uint64_t s, smax = A336641_isqrt(X);
    A336641_sieve(A336641_isqrt(2 * X + 2) + 2);
    A336641_cnt = 0;
    for (s = 1; s <= smax; s++) {
        uint64_t S = s * s, L = X / S, m = s;
        uint64_t ps[20], sig0[20], sig1[20], f0[20][20], f1[20][20];
        int vs[20], n0[20], n1[20], w = 0, i;
        unsigned mask;
        while (m > 1) {
            uint64_t p = A336641_spf[m];
            int v = 0;
            while (m % p == 0) { m /= p; v++; }
            ps[w] = p; vs[w] = v; w++;
        }
        for (i = 0; i < w; i++) {
            uint64_t p = ps[i], pw = 1, sum = 1;
            int j;
            for (j = 0; j < 2 * vs[i]; j++) { pw *= p; sum += pw; }
            sig0[i] = sum;
            n0[i] = A336641_pfac(sum, L, f0[i]);
            if (p <= L) {
                sig1[i] = sum + pw * p;
                n1[i] = A336641_pfac(sig1[i], L / p, f1[i]);
            } else n1[i] = -1;
        }
        for (mask = 0; mask < (1u << w); mask++) {
            uint64_t prodT = 1, N = 1;
            int ok = 1, j, t;
            for (i = 0; i < w; i++) {
                if (mask & (1u << i)) {
                    if (n1[i] < 0 || prodT > L / ps[i]) { ok = 0; break; }
                    prodT *= ps[i];
                    N *= sig1[i];
                } else N *= sig0[i];
            }
            if (!ok) continue;
            A336641_S = S; A336641_prodT = prodT; A336641_N = N;
            A336641_Lp = L / prodT; A336641_s = s;
            A336641_nc = 0;
            for (i = 0; i < w; i++) {
                if (mask & (1u << i))
                    for (j = 0; j < n1[i]; j++) A336641_addcand(f1[i][j]);
                else
                    for (j = 0; j < n0[i]; j++) A336641_addcand(f0[i][j]);
            }
            A336641_addcand(2);
            A336641_addcand(3);
            for (j = 0; j < A336641_nc; j++) {
                uint64_t tmp[20];
                int tc = A336641_pfac(A336641_cand[j] + 1, A336641_Lp, tmp);
                for (t = 0; t < tc; t++) A336641_addcand(tmp[t]);
            }
            for (j = 1; j < A336641_nc; j++) {
                uint64_t v = A336641_cand[j];
                int q = j - 1;
                while (q >= 0 && A336641_cand[q] > v) { A336641_cand[q + 1] = A336641_cand[q]; q--; }
                A336641_cand[q + 1] = v;
            }
            A336641_dfs(0, 1, 1);
        }
    }
    qsort(A336641_terms, A336641_cnt, sizeof(uint64_t), A336641_cmp);
}

seq_t a(idx_t n) {
    if (A336641_X == 0) {
        A336641_run((uint64_t)1 << 16);
        A336641_X = (uint64_t)1 << 16;
    }
    while (A336641_cnt < (size_t)n) {
        uint64_t X = A336641_X * 4;
        A336641_run(X);
        A336641_X = X;
    }
    return (seq_t)A336641_terms[n - 1];
}
