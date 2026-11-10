/*
 * A323585  Third Moebius transform of A000219. Number of plane partitions of
 *          n whose multiset of rows is aperiodic and whose multiset of
 *          columns is also aperiodic and whose parts are relatively prime.
 * https://oeis.org/A323585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d)^{*3} * A000219(d), i.e. the Dirichlet
 *            convolution of A000219 with mu three times; a(0) = 1. The kernel
 *            mu*mu*mu is multiplicative with value (-1)^k*binomial(3,k) at
 *            p^k (0 for k>=4)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 47, u64 n <= 121, u128 n <= 320,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 9516 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A000219_A = NULL;
static uint64_t *A000219_S2 = NULL;
static size_t A000219_cnt = 0;
static size_t A000219_cap = 0;

#define A000219_FIX_N 400
static seq_t A000219_F[A000219_FIX_N + 1];
static int A000219_F_done = 0;

static int A000219_fixed_mode(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 200; i++) x = x * 2;
    return x == 0;
}

static uint64_t A000219_sigma2(uint64_t k)
{
    uint64_t s = 0, d;
    for (d = 1; d * d <= k; d++) {
        if (k % d == 0) {
            uint64_t e = k / d;
            s += d * d;
            if (e != d) s += e * e;
        }
    }
    return s;
}

static void A000219_grow(size_t need)
{
    size_t cap = A000219_cap ? A000219_cap : 64;
    size_t i;
    seq_t *nA;
    uint64_t *nS;
    while (cap < need) cap *= 2;
    nA = mem_new(seq_t, cap);
    for (i = 0; i < cap; i++) nA[i] = 0;
    for (i = 0; i < A000219_cnt; i++) nA[i] = A000219_A[i];
    nS = (uint64_t *)realloc(A000219_S2, cap * sizeof(uint64_t));
    for (i = A000219_cap; i < cap; i++) nS[i] = A000219_sigma2((uint64_t)i);
    if (A000219_A) mem_free(A000219_A);
    A000219_A = nA;
    A000219_S2 = nS;
    A000219_cap = cap;
}

static seq_t A000219_a(idx_t n)
{
    size_t m, k;
    if (n <= A000219_FIX_N && A000219_fixed_mode()) {
        if (!A000219_F_done) {
            size_t j, r;
            for (j = 0; j <= A000219_FIX_N; j++) A000219_F[j] = 0;
            A000219_F[0] = 1;
            for (k = 1; k <= A000219_FIX_N; k++)
                for (r = 0; r < k; r++)
                    for (j = k; j <= A000219_FIX_N; j++)
                        A000219_F[j] += A000219_F[j - k];
            A000219_F_done = 1;
        }
        return A000219_F[n];
    }
    if ((size_t)n + 1 > A000219_cap) A000219_grow((size_t)n + 1);
    for (m = A000219_cnt; m <= (size_t)n; m++) {
        if (m == 0) {
            A000219_A[0] = 1;
        } else {
            seq_t s = 0;
            for (k = 1; k <= m; k++)
                s += A000219_A[m - k] * (seq_t)A000219_S2[k];
            A000219_A[m] = s / (seq_t)m;
        }
        A000219_cnt = m + 1;
    }
    return A000219_A[n];
}

/* kernel mu*mu*mu at m: returns sign in *neg, magnitude as result */
static long long kernel3(uint64_t m, int *neg)
{
    long long mag = 1;
    uint64_t p;
    int sg = 0;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (e >= 4) { *neg = 0; return 0; }
            if (e == 1) { mag *= 3; sg ^= 1; }
            else if (e == 2) { mag *= 3; }
            else { sg ^= 1; }
        }
    }
    if (m > 1) { mag *= 3; sg ^= 1; }
    *neg = sg;
    return mag;
}

seq_t a(idx_t n)
{
    uint64_t d, nn;
    seq_t pos = 0, negs = 0;
    if (n == 0) return (seq_t)1;
    nn = (uint64_t)n;
    (void)A000219_a(n);
    for (d = 1; d <= nn; d++) {
        if (nn % d == 0) {
            int ng;
            long long c = kernel3(nn / d, &ng);
            if (c == 0) continue;
            seq_t t = A000219_a((idx_t)d) * (seq_t)c;
            if (ng) negs += t; else pos += t;
        }
    }
    return pos - negs;
}
