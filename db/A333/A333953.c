/*
 * A333953  Recursively superabundant numbers: numbers m such that
 *          A330575(m)/m > A330575(k)/k for all k < m.
 * https://oeis.org/A333953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th record of R(m) = A330575(m)/m = sum_{e|m}
 *            A074206(e)/e. Computed by search: enumerate all A025487 numbers
 *            (non-increasing prime exponents) up to a bound, using R(m) = 1/2
 *            + sum_{j>=1} 2^-(j+1) * prod_p (sum_{a<=v_p(m)} C(a+j-1,a)
 *            p^-a), sort by size, and keep the strict records
 * Range:     u8 n <= 14, u16 n <= 33, u32 n <= 85, u64 n <= 213,
 *            u128 n <= 213, big any n
 * Validated: standard, pass, 61 terms through n = 196 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

#define RS_NP 30

typedef struct { long double lg; long double r; uint8_t e[RS_NP]; } rs_cand;
typedef struct { uint8_t e[RS_NP]; } rs_term;

static const int RS_PR[RS_NP] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
                                 73,79,83,89,97,101,103,107,109,113};

static rs_term *rs_terms = NULL;
static size_t rs_nterms = 0;
static long double rs_L = 0;

static long double *rs_tab = NULL;
static size_t rs_off[RS_NP + 1];
static size_t rs_stride = 0;
static long double rs_lp[RS_NP];
static rs_cand *rs_c = NULL;
static size_t rs_nc = 0, rs_cap = 0;
static uint8_t rs_cur[RS_NP];

static long double rs_eval(int len)
{
    int k, om = 0;
    size_t j, J;
    long double w = 0.25L, sum = 0.5L;
    for (k = 0; k < len; k++) om += rs_cur[k];
    J = (size_t)(4 * om + 60);
    if (J > rs_stride - 1) J = rs_stride - 1;
    for (j = 1; j <= J; j++) {
        long double pr = w;
        for (k = 0; k < len; k++)
            pr *= rs_tab[(rs_off[k] + rs_cur[k]) * rs_stride + j];
        sum += pr;
        w *= 0.5L;
    }
    return sum;
}

static void rs_gen(int k, int maxe, long double lg)
{
    int e;
    if (rs_nc == rs_cap) {
        rs_cap = rs_cap ? rs_cap * 2 : 4096;
        rs_c = (rs_cand *)realloc(rs_c, rs_cap * sizeof(rs_cand));
    }
    rs_c[rs_nc].lg = lg;
    rs_c[rs_nc].r = rs_eval(k);
    memset(rs_c[rs_nc].e, 0, RS_NP);
    memcpy(rs_c[rs_nc].e, rs_cur, (size_t)k);
    rs_nc++;
    if (k >= RS_NP) return;
    for (e = 1; e <= maxe; e++) {
        if (lg + e * rs_lp[k] > rs_L) break;
        rs_cur[k] = (uint8_t)e;
        rs_gen(k + 1, e, lg + e * rs_lp[k]);
    }
    rs_cur[k] = 0;
}

static int rs_cmp(const void *a, const void *b)
{
    long double x = ((const rs_cand *)a)->lg, y = ((const rs_cand *)b)->lg;
    return x < y ? -1 : (x > y ? 1 : 0);
}

static long double rs_ln(long double x)
{
    /* natural log via series on reduced argument */
    int k = 0;
    long double s = 0, t, t2, term;
    int i;
    const long double LN2 = 0.693147180559945309417232121458L;
    while (x > 1.5L) { x /= 2; k++; }
    while (x < 0.75L) { x *= 2; k--; }
    t = (x - 1) / (x + 1);
    t2 = t * t;
    term = t;
    for (i = 1; i < 80; i += 2) {
        s += term / i;
        term *= t2;
    }
    return 2 * s + k * LN2;
}

static void rs_build(long double L)
{
    int k;
    size_t rows = 0, r, j, Ommax, JT;
    long double th = 0;
    size_t emax[RS_NP];
    long double *t;

    rs_L = L;
    for (k = 0; k < RS_NP; k++) rs_lp[k] = rs_ln((long double)RS_PR[k]);
    for (k = 0; k < RS_NP; k++) {
        th += rs_lp[k];
        emax[k] = (size_t)(L / th) + 2;
        rs_off[k] = rows;
        rows += emax[k] + 1;
    }
    rs_off[RS_NP] = rows;
    Ommax = (size_t)(L / 0.693147L) + 2;
    JT = 4 * Ommax + 61;
    rs_stride = JT + 1;
    free(rs_tab);
    rs_tab = (long double *)malloc(rows * rs_stride * sizeof(long double));
    t = (long double *)malloc(rs_stride * sizeof(long double));
    for (k = 0; k < RS_NP; k++) {
        size_t E;
        long double *row0 = rs_tab + rs_off[k] * rs_stride;
        for (j = 0; j < rs_stride; j++) { row0[j] = 1; t[j] = 1; }
        for (E = 1; E <= emax[k]; E++) {
            long double *row = rs_tab + (rs_off[k] + E) * rs_stride;
            long double *prev = row - rs_stride;
            for (j = 0; j < rs_stride; j++) {
                if (j == 0) t[j] = 1;
                else t[j] = t[j] * (long double)(E + j - 1) / (long double)E / (long double)RS_PR[k];
                row[j] = prev[j] + (j == 0 ? 0 : t[j]);
            }
            row[0] = 1;
        }
        /* t for j=0 handled: C(a-1,a)=0 for a>=1; row[0] is irrelevant */
    }
    free(t);
    (void)r;

    rs_nc = 0;
    memset(rs_cur, 0, sizeof(rs_cur));
    rs_gen(0, 1000, 0);
    qsort(rs_c, rs_nc, sizeof(rs_cand), rs_cmp);

    free(rs_terms);
    rs_terms = (rs_term *)malloc((rs_nc ? rs_nc : 1) * sizeof(rs_term));
    rs_nterms = 0;
    {
        long double best = -1;
        size_t i;
        for (i = 0; i < rs_nc; i++) {
            if (rs_c[i].r > best) {
                memcpy(rs_terms[rs_nterms].e, rs_c[i].e, RS_NP);
                rs_nterms++;
                best = rs_c[i].r;
            }
        }
    }
    free(rs_c);
    rs_c = NULL; rs_nc = 0; rs_cap = 0;
}

seq_t a(idx_t n)
{
    seq_t res = 1;
    int k;
    unsigned e;
    if (n < 1) return 0;
    while (rs_nterms < n) {
        long double L = rs_L == 0 ? 30.0L : rs_L * 1.4L;
        if (L > 95.0L) {
            if (rs_L >= 95.0L) return 0;
            L = 95.0L;
        }
        rs_build(L);
    }
    for (k = 0; k < RS_NP; k++)
        for (e = 0; e < rs_terms[n - 1].e[k]; e++)
            res *= (seq_t)RS_PR[k];
    return res;
}
