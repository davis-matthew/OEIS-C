/*
 * A324905  a(n) = A007895(A003965(n)).
 * https://oeis.org/A324905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007895(A003965(n)), where A003965(n) = Product
 *            Fibonacci(k+2)^e over n = Product prime(k)^e. The product is
 *            built in a private multi-limb integer, because it overflows 64
 *            bits even when a(n) is small. The Zeckendorf terms are then
 *            counted greedily
 * Range:     u8 n <= 17711, u16 n <= 17711, u32 n <= 17711, u64 n <= 17711,
 *            u128 n <= 17711, big any n
 * Validated: standard, pass, 121 terms through n = 8872 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static int A324905_cmp(const uint32_t *x, const uint32_t *y, size_t L) {
    for (size_t i = L; i-- > 0; ) {
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    }
    return 0;
}

static void A324905_add(uint32_t *r, const uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t c = 0;
    for (size_t i = 0; i < L; i++) {
        c += (uint64_t)x[i] + y[i];
        r[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void A324905_sub(uint32_t *r, const uint32_t *x, const uint32_t *y, size_t L) {
    int64_t br = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t d = (int64_t)x[i] - (int64_t)y[i] - br;
        if (d < 0) { d += ((int64_t)1 << 32); br = 1; } else br = 0;
        r[i] = (uint32_t)d;
    }
}

static void A324905_mul(uint32_t *r, const uint32_t *x, const uint32_t *y, size_t L, uint32_t *tmp) {
    memset(tmp, 0, L * sizeof(uint32_t));
    for (size_t i = 0; i < L; i++) {
        if (x[i] == 0) continue;
        uint64_t c = 0;
        for (size_t j = 0; i + j < L; j++) {
            uint64_t cur = (uint64_t)x[i] * y[j] + tmp[i + j] + c;
            tmp[i + j] = (uint32_t)cur;
            c = cur >> 32;
        }
    }
    memcpy(r, tmp, L * sizeof(uint32_t));
}

static bool A324905_iszero(const uint32_t *x, size_t L) {
    for (size_t i = 0; i < L; i++) if (x[i]) return false;
    return true;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;

    /* sieve primes up to n to get prime indices */
    unsigned char *comp = (unsigned char *)calloc((size_t)n + 1, 1);
    for (idx_t p = 2; p * p <= n; p++)
        if (!comp[p])
            for (idx_t j = p * p; j <= n; j += p) comp[j] = 1;

    idx_t ks[64], es[64];
    int nf = 0;
    idx_t temp = n, k = 0;
    for (idx_t p = 2; p <= n && temp > 1; p++) {
        if (comp[p]) continue;
        k++;
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) { temp /= p; e++; }
            ks[nf] = k; es[nf] = e; nf++;
        }
    }
    free(comp);

    /* Fibonacci(m) < 2^m, so total bits <= sum e*(k+2) */
    idx_t total = 0;
    for (int i = 0; i < nf; i++) total += es[i] * (ks[i] + 2);
    size_t L = (size_t)(total / 32 + 3);

    uint32_t *N = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *A = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *W = (uint32_t *)calloc(L, sizeof(uint32_t));
    N[0] = 1;

    for (int i = 0; i < nf; i++) {
        idx_t m = ks[i] + 2;
        memset(A, 0, L * sizeof(uint32_t));
        memset(B, 0, L * sizeof(uint32_t));
        B[0] = 1; /* A = F(0) = 0, B = F(1) = 1 */
        for (idx_t s = 0; s < m; s++) {
            A324905_add(T, A, B, L);
            memcpy(A, B, L * sizeof(uint32_t));
            memcpy(B, T, L * sizeof(uint32_t));
        }
        /* A = F(m) */
        for (idx_t e = 0; e < es[i]; e++) A324905_mul(N, N, A, L, W);
    }

    /* Zeckendorf greedy: fibs 1, 2, 3, 5, ... */
    memset(A, 0, L * sizeof(uint32_t));
    memset(B, 0, L * sizeof(uint32_t));
    A[0] = 1; B[0] = 2;
    while (A324905_cmp(B, N, L) <= 0) {
        A324905_add(T, A, B, L);
        memcpy(A, B, L * sizeof(uint32_t));
        memcpy(B, T, L * sizeof(uint32_t));
    }
    long long count = 0;
    while (!A324905_iszero(A, L) && !A324905_iszero(N, L)) {
        if (A324905_cmp(A, N, L) <= 0) {
            A324905_sub(N, N, A, L);
            count++;
        }
        /* (A, B) -> (B - A, A) */
        A324905_sub(T, B, A, L);
        memcpy(B, A, L * sizeof(uint32_t));
        memcpy(A, T, L * sizeof(uint32_t));
    }

    free(N); free(A); free(B); free(T); free(W);
    return (seq_t)count;
}
