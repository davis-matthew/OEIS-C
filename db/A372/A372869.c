/*
 * A372869  Decimal expansion of the number whose continued fraction
 *          coefficients are given in A084580.
 * https://oeis.org/A372869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = n-th decimal digit of x = [0; A084580(1), A084580(2),
 *            ...]; A084580 is built greedily (pick k minimising
 *            (count(k)+1)/GK(k)), then the convergent p/q is formed with
 *            big-limb arithmetic and expanded by long division
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 151 terms through n = 9386 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

static double A372869_gk(uint64_t k)
{
    return log1p(1.0 / ((double)k * ((double)k + 2.0))) / log(2.0);
}

static uint8_t *A372869_digits = NULL;
static uint64_t A372869_have = 0;

static void A372869_build(uint64_t D)
{
    uint64_t cap = 2 * D + 600;
    uint32_t *cnt = (uint32_t *)calloc(cap + 2, sizeof(uint32_t));
    double *pk = (double *)calloc(cap + 2, sizeof(double));
    size_t L = (size_t)(D / 8 + 200);
    uint32_t *pp = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *pc = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *qp = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *qc = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L + 3, sizeof(uint32_t));
    uint8_t *dg = (uint8_t *)malloc((size_t)D + 1);
    /* p_{-1}=1, p_0=0 ; q_{-1}=0, q_0=1 */
    pp[0] = 1; pc[0] = 0;
    qp[0] = 0; qc[0] = 1;
    size_t lp = 1, lq = 1;
    uint64_t target_bits = (uint64_t)(1.7 * (double)D) + 96;
    uint64_t terms = 0;

    for (;;) {
        /* next greedy term */
        uint64_t bk = 1;
        if (pk[1] == 0.0) pk[1] = A372869_gk(1);
        double br = (cnt[1] + 1.0) / pk[1];
        for (uint64_t k = 2; k < cap; ++k) {
            if (pk[k] == 0.0) pk[k] = A372869_gk(k);
            if (1.0 / pk[k] > br) break;
            double rr = (cnt[k] + 1.0) / pk[k];
            if (rr < br) { br = rr; bk = k; }
        }
        cnt[bk]++;
        terms++;
        /* update convergents: new = a*cur + prev, stored in prev arrays */
        uint64_t a = bk;
        uint64_t c = 0;
        size_t i;
        for (i = 0; i < lp || c; ++i) {
            uint64_t v = c + (i < lp ? (uint64_t)pc[i] * a : 0) + pp[i];
            pp[i] = (uint32_t)v; c = v >> 32;
        }
        if (i > lp) lp = i;
        c = 0;
        for (i = 0; i < lq || c; ++i) {
            uint64_t v = c + (i < lq ? (uint64_t)qc[i] * a : 0) + qp[i];
            qp[i] = (uint32_t)v; c = v >> 32;
        }
        if (i > lq) lq = i;
        { uint32_t *t = pp; pp = pc; pc = t; t = qp; qp = qc; qc = t; }
        uint32_t top = qc[lq - 1];
        uint64_t bits = 32 * (uint64_t)(lq - 1);
        while (top) { bits++; top >>= 1; }
        if (bits >= target_bits || terms + 2 >= cap || lq + 2 >= L) break;
    }
    /* long division of pc/qc (pc < qc) */
    size_t n = lq + 1;
    for (size_t j = 0; j < n + 1; ++j) r[j] = 0;
    for (size_t j = 0; j < lp; ++j) r[j] = pc[j];
    qc[lq] = 0;
    for (uint64_t d = 0; d < D; ++d) {
        uint64_t c = 0;
        for (size_t j = 0; j < n; ++j) {
            uint64_t v = (uint64_t)r[j] * 10 + c;
            r[j] = (uint32_t)v; c = v >> 32;
        }
        int dig = 0;
        for (;;) {
            bool ge = true;
            for (size_t j = n; j-- > 0;) {
                if (r[j] != qc[j]) { ge = r[j] > qc[j]; break; }
            }
            if (!ge) break;
            uint64_t bw = 0;
            for (size_t j = 0; j < n; ++j) {
                uint64_t v = (uint64_t)r[j] - qc[j] - bw;
                r[j] = (uint32_t)v; bw = (v >> 63) & 1;
            }
            dig++;
            if (dig > 9) break;
        }
        dg[d] = (uint8_t)dig;
    }
    free(cnt); free(pk); free(pp); free(pc); free(qp); free(qc); free(r);
    free(A372869_digits);
    A372869_digits = dg;
    A372869_have = D;
}

seq_t a(idx_t n)
{
    if (n >= A372869_have) {
        uint64_t D = (uint64_t)n + 1;
        if (D < 2 * A372869_have) D = 2 * A372869_have;
        if (D < 128) D = 128;
        A372869_build(D);
    }
    return (seq_t)A372869_digits[n];
}
