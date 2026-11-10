/*
 * A398347  Number of S_3-symmetric 3-dimensional vector partitions of (n, n,
 *          n) into strictly distinct parts.
 * https://oeis.org/A398347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} b(k) * a(n-k), a(0) = 1, where b(k)
 *            = Sum_{d|k} (-1)^(k/d - 1) * d * A400564(d) (G.f.:
 *            Product_{w>=1} (1 + x^w)^A400564(w))
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 57, u64 n <= 151, u128 n <= 407,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 9017 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* A400564(n), computed natively (small values) */
static idx_t A400564_c(idx_t n)
{
    idx_t r = 2 + n / 2;
    if (n % 3 == 0)
        r -= 1;
    if (n % 2 == 0)
        r += (n * n + 24) / 48;
    return r;
}

static bool seq_is_unbounded(void)
{
    seq_t x = (seq_t)1;
    int i;
    for (i = 0; i < 300; i++)
        x += x;
    return x != (seq_t)0;
}

static seq_t *memo_vals = 0;
static idx_t memo_cnt = 0;
static bool memo_overflow = false;

seq_t a(idx_t n)
{
    if (n < memo_cnt)
        return memo_vals[n];

    idx_t N = n;
    bool unb = seq_is_unbounded();

    /* b(k) as native unsigned (always nonnegative, fits easily) */
    idx_t *b = (idx_t *)malloc(((size_t)N + 1) * sizeof(idx_t));
    idx_t d, k, i;
    for (k = 0; k <= N; k++)
        b[k] = 0;
    for (d = 1; d <= N; d++) {
        idx_t t = d * A400564_c(d);
        idx_t m = 1;
        for (k = d; k <= N; k += d, m++) {
            if (m & 1)
                b[k] += t;
            else
                b[k] -= t;
        }
    }

    seq_t *res = mem_new(seq_t, (size_t)N + 1);
    for (i = 0; i < memo_cnt; i++)
        res[i] = memo_vals[i];
    if (memo_vals)
        mem_free(memo_vals);
    memo_vals = res;

    idx_t start = memo_cnt;
    if (start == 0) {
        res[0] = (seq_t)1;
        start = 1;
    }

    for (i = start; i <= N; i++) {
        if (unb) {
            seq_t S = (seq_t)0;
            for (k = 1; k <= i; k++)
                S += res[i - k] * b[k];
            res[i] = S / i;
        } else if (memo_overflow) {
            res[i] = res[i - 1];   /* value no longer representable anyway */
        } else {
            /* sum = i*Q + R exactly, avoiding overflow of i*a(i) */
            seq_t Q = (seq_t)0;
            idx_t R = 0;
            for (k = 1; k <= i; k++) {
                seq_t A = res[i - k];
                idx_t bq = b[k] / i, br = b[k] % i;
                seq_t Aq = A / i;
                idx_t Ar = (idx_t)(A % i);
                Q += A * bq;
                Q += Aq * br;
                R += br * Ar;
            }
            res[i] = Q + (seq_t)(R / i);
            if (res[i] < res[i - 1])
                memo_overflow = true;
        }
    }

    free(b);
    memo_cnt = N + 1;
    return memo_vals[n];
}
