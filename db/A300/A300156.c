/*
 * A300156  Indices of records in A299990.
 * https://oeis.org/A300156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) is the n-th m at which A299990(m) = A010846(m) -
 *            2*A000005(m) sets a new record. Numbers are factored with a
 *            segmented sieve. A simplex lattice-point bound, A010846(m) <=
 *            (log m + Sum log p)^k / (k! * Prod log p), skips most m that
 *            cannot set a record
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Validated: standard, pass, 61 terms through n = 104 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#define A300156_BLK 65536

static idx_t   *A300156_recs = NULL;
static size_t   A300156_nrec = 0, A300156_cap = 0;
static uint64_t A300156_next = 1;          /* next m to scan */
static long long A300156_best = 0;
static int      A300156_started = 0;

static uint32_t *A300156_pr = NULL;
static double   *A300156_lp = NULL;
static size_t    A300156_np = 0;
static uint64_t  A300156_plim = 0;

static void A300156_sieve(uint64_t lim) {
    char *c = (char *)calloc((size_t)lim + 1, 1);
    size_t cnt = 0;
    for (uint64_t i = 2; i <= lim; ++i) {
        if (!c[i]) { ++cnt; for (uint64_t j = i * i; j <= lim; j += i) c[j] = 1; }
    }
    free(A300156_pr); free(A300156_lp);
    A300156_pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    A300156_lp = (double *)malloc(cnt * sizeof(double));
    A300156_np = 0;
    for (uint64_t i = 2; i <= lim; ++i)
        if (!c[i]) {
            A300156_pr[A300156_np] = (uint32_t)i;
            A300156_lp[A300156_np] = log((double)i);
            ++A300156_np;
        }
    free(c);
    A300156_plim = lim;
}

/* count numbers <= lim of the form cur * prod_{j<=idx} pr[j]^e_j */
static long long A300156_cnt(int idx, uint64_t cur, const uint64_t *pr, uint64_t lim) {
    uint64_t q = lim / cur;
    if (idx == 0) {
        if (pr[0] == 2) return 64 - __builtin_clzll(q);
        long long c = 0; uint64_t v = 1;
        while (1) { ++c; if (v > q / pr[0]) break; v *= pr[0]; }
        return c;
    }
    long long c = 0; uint64_t v = cur;
    while (1) {
        c += A300156_cnt(idx - 1, v, pr, lim);
        if (v > lim / pr[idx]) break;
        v *= pr[idx];
    }
    return c;
}

static void A300156_push(uint64_t m) {
    if (A300156_nrec == A300156_cap) {
        A300156_cap = A300156_cap ? 2 * A300156_cap : 256;
        A300156_recs = (idx_t *)realloc(A300156_recs, A300156_cap * sizeof(idx_t));
    }
    A300156_recs[A300156_nrec++] = (idx_t)m;
}

static void A300156_block(void) {
    static uint64_t rem[A300156_BLK];
    static unsigned char kk[A300156_BLK];
    static double sl[A300156_BLK], pl[A300156_BLK];
    static long long dd[A300156_BLK];
    static const double fact[20] = {1,1,2,6,24,120,720,5040,40320,362880,3628800,
        39916800.0,479001600.0,6227020800.0,87178291200.0,1307674368000.0,
        20922789888000.0,355687428096000.0,6402373705728000.0,121645100408832000.0};
    uint64_t lo = A300156_next, hi = lo + A300156_BLK - 1;
    uint64_t sq = (uint64_t)sqrt((double)hi) + 2;
    if (sq > A300156_plim) A300156_sieve(sq * 2 > 65536 ? sq * 2 : 65536);

    for (uint64_t i = 0; i < A300156_BLK; ++i) {
        rem[i] = lo + i; kk[i] = 0; sl[i] = 0.0; pl[i] = 1.0; dd[i] = 1;
    }
    for (size_t t = 0; t < A300156_np; ++t) {
        uint64_t p = A300156_pr[t];
        if (p * p > hi) break;
        double lp = A300156_lp[t];
        uint64_t s = ((lo + p - 1) / p) * p;
        for (uint64_t m = s; m <= hi; m += p) {
            uint64_t i = m - lo;
            int e = 0;
            while (rem[i] % p == 0) { rem[i] /= p; ++e; }
            kk[i]++; sl[i] += lp; pl[i] *= lp; dd[i] *= (e + 1);
        }
    }
    for (uint64_t i = 0; i < A300156_BLK; ++i) {
        uint64_t m = lo + i;
        if (rem[i] > 1) {
            double lp = log((double)rem[i]);
            kk[i]++; sl[i] += lp; pl[i] *= lp; dd[i] *= 2;
        }
        if (m == 1) {
            long long v = -1;
            if (!A300156_started) { A300156_started = 1; A300156_best = v; A300156_push(1); }
            continue;
        }
        int k = kk[i];
        double L = log((double)m);
        double B = pow(L + sl[i], (double)k) / (fact[k] * pl[i]);
        if (B * (1.0 + 1e-9) + 1.0 - 2.0 * (double)dd[i] <= (double)A300156_best) continue;
        /* exact evaluation */
        uint64_t pr[20]; int c = 0; uint64_t r = m;
        for (size_t t = 0; t < A300156_np; ++t) {
            uint64_t p = A300156_pr[t];
            if (p * p > r) break;
            if (r % p == 0) { pr[c++] = p; while (r % p == 0) r /= p; }
        }
        if (r > 1) pr[c++] = r;
        long long S = A300156_cnt(c - 1, 1, pr, m);
        long long v = S - 2 * dd[i];
        if (v > A300156_best) { A300156_best = v; A300156_push(m); }
    }
    A300156_next = hi + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (A300156_nrec < (size_t)n) A300156_block();
    return (seq_t)A300156_recs[n - 1];
}
