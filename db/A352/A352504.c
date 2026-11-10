/*
 * A352504  Numbers k > 1 such that log(A005179(k))/k is a record low.
 * https://oeis.org/A352504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. For k = 2, 3, ..., compute L(k) = log(A005179(k)) as the
 *            minimum of sum (d_i - 1)*log(p_i) over factorizations k =
 *            d_1*d_2*... with d_1 >= d_2 >= ... >= 2, where p_i is the i-th
 *            prime. Keep k when L(k)/k is a new record low
 * Range:     u8 n <= 39, u16 n <= 244, u32 n <= 442, u64 n <= 442,
 *            u128 n <= 442, big any n
 * Validated: standard, pass, 73 terms through n = 380 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/mem.h"

#define A352504_NP 64

static const int a352504_primes[A352504_NP] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
    157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
    239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311
};

static double a352504_lp[A352504_NP];
static bool a352504_init = false;

static uint32_t *a352504_spf = NULL;
static uint64_t a352504_spf_lim = 0;

static uint64_t a352504_div[8192];
static int a352504_ndiv = 0;
static double a352504_best;

static void a352504_ensure_spf(uint64_t k)
{
    if (k <= a352504_spf_lim) return;
    uint64_t lim = a352504_spf_lim ? a352504_spf_lim : 65536;
    while (lim < k) lim *= 2;
    free(a352504_spf);
    a352504_spf = (uint32_t *)calloc((size_t)(lim + 1), sizeof(uint32_t));
    for (uint64_t i = 2; i <= lim; i++) {
        if (a352504_spf[i] != 0) continue;
        a352504_spf[i] = (uint32_t)i;
        for (uint64_t j = i * i; j <= lim; j += i)
            if (a352504_spf[j] == 0) a352504_spf[j] = (uint32_t)i;
    }
    a352504_spf_lim = lim;
}

static int a352504_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void a352504_divisors(uint64_t k)
{
    a352504_ndiv = 1;
    a352504_div[0] = 1;
    uint64_t m = k;
    while (m > 1) {
        uint64_t p = a352504_spf[m];
        int e = 0;
        while (m % p == 0) { m /= p; e++; }
        int cur = a352504_ndiv;
        uint64_t pw = 1;
        for (int t = 1; t <= e; t++) {
            pw *= p;
            for (int j = 0; j < cur; j++)
                a352504_div[a352504_ndiv++] = a352504_div[j] * pw;
        }
    }
    qsort(a352504_div, (size_t)a352504_ndiv, sizeof(uint64_t), a352504_cmp);
}

static void a352504_rec(uint64_t n, int i, uint64_t maxd, double cur)
{
    if (n == 1) {
        if (cur < a352504_best) a352504_best = cur;
        return;
    }
    if (i >= A352504_NP) return;
    for (int j = 1; j < a352504_ndiv; j++) {
        uint64_t d = a352504_div[j];
        if (d > maxd || d > n) break;
        if (n % d != 0) continue;
        double v = cur + (double)(d - 1) * a352504_lp[i];
        if (v >= a352504_best) break;
        a352504_rec(n / d, i + 1, d, v);
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t next_k = 2;
    static double best_ratio = 1e300;

    if (!a352504_init) {
        for (int i = 0; i < A352504_NP; i++)
            a352504_lp[i] = log((double)a352504_primes[i]);
        a352504_init = true;
    }

    while (len < (size_t)n) {
        uint64_t k = next_k++;
        a352504_ensure_spf(k);
        a352504_divisors(k);
        double thr = (best_ratio >= 1e299) ? 1e300 : best_ratio * (double)k;
        a352504_best = thr;
        a352504_rec(k, 0, k, 0.0);
        if (a352504_best < thr) {
            double r = a352504_best / (double)k;
            if (r < best_ratio) {
                best_ratio = r;
                if (len == cap) {
                    size_t nc = cap ? cap * 2 : 64;
                    seq_t *t = mem_resize(seq_t, cache, nc);
                    if (!t) return (seq_t)0;
                    cache = t;
                    cap = nc;
                }
                cache[len++] = (seq_t)k;
            }
        }
    }
    return cache[n - 1];
}
