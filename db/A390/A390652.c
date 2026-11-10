/*
 * A390652  Denominators of the convergents given by treating A084580 as
 *          continued fraction coefficients after the leading 0.
 * https://oeis.org/A390652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(0)=1, a(1)=c(1), where
 *            c(k)=A084580(k) (continued fraction [0; c(1), c(2), ...]); c is
 *            generated greedily: choose k minimizing (count(k)+1)/GK(k),
 *            GK(k)=log(1+1/(k(k+2)))/log 2, ties to smaller k
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 29, u64 n <= 54, u128 n <= 101,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 1927 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/mem.h"

static uint32_t *A390652_c = NULL;
static uint32_t *A390652_cnt = NULL;
static double *A390652_p = NULL;
static size_t A390652_len = 0;
static size_t A390652_cap = 0;

static void A390652_grow(size_t need)
{
    if (need <= A390652_cap) return;
    size_t nc = A390652_cap ? A390652_cap * 2 : 64;
    while (nc < need) nc *= 2;
    A390652_c = (uint32_t *)realloc(A390652_c, (nc + 2) * sizeof(uint32_t));
    A390652_cnt = (uint32_t *)realloc(A390652_cnt, (nc + 2) * sizeof(uint32_t));
    A390652_p = (double *)realloc(A390652_p, (nc + 2) * sizeof(double));
    for (size_t k = (A390652_cap ? A390652_cap + 1 : 0); k <= nc + 1; k++) {
        A390652_cnt[k] = 0;
        A390652_c[k] = 0;
        A390652_p[k] = (k == 0) ? 1.0
            : log1p(1.0 / ((double)k * ((double)k + 2.0))) / log(2.0);
    }
    A390652_cap = nc;
}

static void A390652_fill(size_t n)
{
    A390652_grow(n);
    while (A390652_len < n) {
        size_t t = A390652_len + 1;
        uint32_t bk = 1;
        double br = ((double)A390652_cnt[1] + 1.0) / A390652_p[1];
        for (size_t k = 2; k <= t; k++) {
            double r = ((double)A390652_cnt[k] + 1.0) / A390652_p[k];
            if (r < br) { br = r; bk = (uint32_t)k; }
        }
        A390652_c[t] = bk;
        A390652_cnt[bk]++;
        A390652_len = t;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    A390652_fill((size_t)n);
    seq_t q0 = 1, q1 = (seq_t)A390652_c[1];
    for (size_t i = 2; i <= (size_t)n; i++) {
        seq_t q = (seq_t)A390652_c[i] * q1 + q0;
        q0 = q1;
        q1 = q;
    }
    return q1;
}
