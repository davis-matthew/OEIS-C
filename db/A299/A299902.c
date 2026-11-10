/*
 * A299902  Partial sums of A033616.
 * https://oeis.org/A299902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A033616(k); A033616 has a rational g.f.
 *            P(x)/Q(x), so its terms follow a linear recurrence of order 23
 *            and a(n) is their running sum
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void A299902_poly_mul(const long long *x, size_t la,
                             const long long *y, size_t lb,
                             long long *res)
{
    size_t lr = la + lb - 1;
    for (size_t i = 0; i < lr; ++i) res[i] = 0;
    for (size_t i = 0; i < la; ++i)
        for (size_t j = 0; j < lb; ++j)
            res[i + j] += x[i] * y[j];
}

static long long A299902_Q[24];
static long long A299902_P[24];
static bool A299902_ready = false;

static void A299902_init(void)
{
    if (A299902_ready) return;
    long long t[40], tmp2[40];
    const long long Q1[4] = { 1, -3, 3, -1 };
    const long long Q2[5] = { 1, -1, 1, -1, 1 };
    const long long Q3[5] = { 1, 1, 1, 1, 1 };
    const long long Q4[7] = { 1, 0, 0, 1, 0, 0, 1 };
    const long long Q5[7] = { 1, 1, 1, 1, 1, 1, 1 };
    A299902_poly_mul(Q1, 4, Q2, 5, t);
    A299902_poly_mul(t, 8, Q3, 5, tmp2);
    A299902_poly_mul(tmp2, 12, Q4, 7, t);
    A299902_poly_mul(t, 18, Q5, 7, tmp2);
    for (int i = 0; i < 24; ++i) A299902_Q[i] = tmp2[i];

    const long long R[19] = { 1, -1, 2, -1, 3, -1, 4, -1,
                              4, -1, 4, -1, 4, -1, 3,
                              -1, 2, -1, 1 };
    const long long B0[3] = { 1, 0, 1 };
    const long long B1[4] = { 1, 3, 3, 1 };
    A299902_poly_mul(R, 19, B0, 3, t);
    A299902_poly_mul(t, 21, B1, 4, tmp2);
    for (int i = 0; i < 24; ++i) A299902_P[i] = tmp2[i];
    A299902_ready = true;
}

seq_t a(idx_t n)
{
    static idx_t cap = 0;
    static idx_t size = 0;
    static seq_t *av = NULL;   /* A033616 terms */
    static seq_t *sv = NULL;   /* partial sums */
    const int K = 23;

    A299902_init();

    if (n >= cap) {
        idx_t nc = cap ? cap : 64;
        while (nc <= n) nc <<= 1;
        av = mem_resize(seq_t, av, nc);
        sv = mem_resize(seq_t, sv, nc);
        cap = nc;
    }

    for (idx_t i = size; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        int maxk = (i < (idx_t)K) ? (int)i : K;
        for (int j = 1; j <= maxk; ++j) {
            long long c = A299902_Q[j];
            if (c > 0) pos += (seq_t)c * av[i - j];
            else if (c < 0) neg += (seq_t)(-c) * av[i - j];
        }
        /* a[i] = p[i] - sum q_j a[i-j] = p[i] + neg - pos  (p[i]=0 for i>23) */
        seq_t base = neg;
        if (i <= (idx_t)K) {
            long long p = A299902_P[i];
            if (p > 0) base += (seq_t)p;
            else if (p < 0) pos += (seq_t)(-p);
        }
        av[i] = base - pos;
        sv[i] = (i == 0) ? av[i] : (seq_t)(sv[i - 1] + av[i]);
    }
    if (n + 1 > size) size = n + 1;
    return sv[n];
}
