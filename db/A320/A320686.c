/*
 * A320686  Inverse Euler transform of A004010.
 * https://oeis.org/A320686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/n) * Sum_{d|n} mu(n/d) * c(d), where c(n) = n*b(n) -
 *            Sum_{k=1..n-1} c(k)*b(n-k) and b = A004010 (inverse Euler
 *            transform: Product_{k>=1} (1-q^k)^(-a(k)) = Sum b(n) q^n). For
 *            builds where seq_t is 64 bits or narrower, c(n) is held in
 *            128-bit integers, so the step that multiplies by n doe...
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 14, i128 n <= 26,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 668 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* ---- A004010 (values are small: ~20*n^5, so they fit in long long) ---- */
static long long *A004010_P = NULL;  /* A004010_P[k] = coeff of q^(k+1) in eta(q)^6 eta(q^3)^6 */
static idx_t A004010_PL = 0;

static void A004010_build(idx_t len) {
    long long *AB = (long long *)calloc((size_t)len, sizeof(long long));
    long long *NP = (long long *)calloc((size_t)len, sizeof(long long));
    long long L = (long long)len;
    for (long long m = 0; m * (m + 1) / 2 < L; m++) {
        long long t = m * (m + 1) / 2;
        long long sa = (m & 1) ? -(2 * m + 1) : (2 * m + 1);
        for (long long k = 0; t + 3 * (k * (k + 1) / 2) < L; k++) {
            long long u = 3 * (k * (k + 1) / 2);
            long long sb = (k & 1) ? -(2 * k + 1) : (2 * k + 1);
            AB[t + u] += sa * sb;
        }
    }
    for (long long i = 0; i < L; i++) {
        if (AB[i] == 0) continue;
        for (long long j = 0; i + j < L; j++)
            NP[i + j] += AB[i] * AB[j];
    }
    free(AB);
    free(A004010_P);
    A004010_P = NP;
    A004010_PL = len;
}

static long long A004010_sig5(idx_t m) {
    long long s = 0;
    for (idx_t d = 1; d * d <= m; d++) {
        if (m % d) continue;
        idx_t e = m / d;
        long long x = (long long)d;
        s += x * x * x * x * x;
        if (e != d) {
            long long y = (long long)e;
            s += y * y * y * y * y;
        }
    }
    return s;
}

static long long A004010_ll(idx_t n) {
    if (n == 0) return 1;
    if (n > A004010_PL) {
        idx_t len = 2 * A004010_PL;
        if (len < n) len = n;
        if (len < 64) len = 64;
        A004010_build(len);
    }
    long long S = A004010_sig5(n);
    if (n % 3 == 0) S -= 27 * A004010_sig5(n / 3);
    long long c = A004010_P[n - 1];
    return (S - c) / 13 * 252;
}

/* ---- A320686 ---- */
static int A320686_mu(idx_t m) {
    int r = 1;
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            r = -r;
        }
    }
    if (m > 1) r = -r;
    return r;
}

/* narrow path: 128-bit intermediates (wrapping unsigned arithmetic) */
static unsigned __int128 *A320686_CN = NULL;
static idx_t A320686_capN = 0;

static void A320686_build_narrow(idx_t cap) {
    long long *B = (long long *)calloc((size_t)cap + 1, sizeof(long long));
    unsigned __int128 *C = (unsigned __int128 *)calloc((size_t)cap + 1, sizeof(unsigned __int128));
    for (idx_t i = 1; i <= cap; i++) B[i] = A004010_ll(i);
    for (idx_t i = 1; i <= cap; i++) {
        unsigned __int128 s = (unsigned __int128)i * (unsigned __int128)(__int128)B[i];
        for (idx_t k = 1; k < i; k++)
            s -= C[k] * (unsigned __int128)(__int128)B[i - k];
        C[i] = s;
    }
    free(B);
    free(A320686_CN);
    A320686_CN = C;
    A320686_capN = cap;
}

/* wide path: exact arithmetic in seq_t */
static seq_t *A320686_CW = NULL;
static idx_t A320686_capW = 0;

static void A320686_build_wide(idx_t cap) {
    seq_t *B = mem_new(seq_t, (size_t)cap + 1);
    seq_t *C = mem_new(seq_t, (size_t)cap + 1);
    for (idx_t i = 1; i <= cap; i++) B[i] = (seq_t)A004010_ll(i);
    for (idx_t i = 1; i <= cap; i++) {
        seq_t s = (seq_t)i * B[i];
        for (idx_t k = 1; k < i; k++) s -= C[k] * B[i - k];
        C[i] = s;
    }
    mem_free(B);
    if (A320686_CW) mem_free(A320686_CW);
    A320686_CW = C;
    A320686_capW = cap;
}

seq_t a(idx_t n) {
    if (sizeof(seq_t) <= 8) {
        if (n > A320686_capN) {
            idx_t cap = 2 * A320686_capN;
            if (cap < n) cap = n;
            if (cap < 64) cap = 64;
            A320686_build_narrow(cap);
        }
        unsigned __int128 s = 0;
        for (idx_t d = 1; d <= n; d++) {
            if (n % d) continue;
            int mu = A320686_mu(n / d);
            if (mu == 1) s += A320686_CN[d];
            else if (mu == -1) s -= A320686_CN[d];
        }
        __int128 r = (__int128)s / (__int128)n;
        return (seq_t)(long long)r;
    } else {
        if (n > A320686_capW) {
            idx_t cap = 2 * A320686_capW;
            if (cap < n) cap = n;
            if (cap < 64) cap = 64;
            A320686_build_wide(cap);
        }
        seq_t s = 0;
        for (idx_t d = 1; d <= n; d++) {
            if (n % d) continue;
            int mu = A320686_mu(n / d);
            if (mu == 1) s += A320686_CW[d];
            else if (mu == -1) s -= A320686_CW[d];
        }
        return s / (seq_t)n;
    }
}
