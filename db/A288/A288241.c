/*
 * A288241  Decimal expansion of the limiting ratio of consecutive terms of
 *          A288231.
 * https://oeis.org/A288241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th decimal digit of 1/x0, where x0 is the smallest
 *            positive root of Sum_{k>=0} floor((k+1)*4^(1/3))*(-x)^k = 0 (the
 *            limit of A288231(n+1)/A288231(n)); x0 is found by Newton's
 *            method in base-1e9 fixed-point arithmetic
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define R288241_BASE 1000000000u

static uint64_t r288241_cbfloor(uint64_t x) {
    uint64_t lo = 0, hi = 1ULL << 21;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 c = (unsigned __int128)mid * mid * mid;
        if (c <= x) lo = mid; else hi = mid - 1;
    }
    return lo;
}

static int r288241_cmp(const uint32_t *a, const uint32_t *b, int L) {
    for (int i = L - 1; i >= 0; --i) {
        if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1;
    }
    return 0;
}

static void r288241_addm(uint32_t *r, const uint32_t *a, const uint32_t *b, int L) {
    uint32_t c = 0;
    for (int i = 0; i < L; ++i) {
        uint32_t t = a[i] + b[i] + c;
        if (t >= R288241_BASE) { t -= R288241_BASE; c = 1; } else c = 0;
        r[i] = t;
    }
}

static void r288241_subm(uint32_t *r, const uint32_t *a, const uint32_t *b, int L) {
    int64_t br = 0;
    for (int i = 0; i < L; ++i) {
        int64_t t = (int64_t)a[i] - (int64_t)b[i] - br;
        if (t < 0) { t += R288241_BASE; br = 1; } else br = 0;
        r[i] = (uint32_t)t;
    }
}

static void r288241_sadd(int *rn, uint32_t *r, int an, const uint32_t *a,
                         int bn, const uint32_t *b, int L) {
    if (an == bn) { r288241_addm(r, a, b, L); *rn = an; }
    else if (r288241_cmp(a, b, L) >= 0) { r288241_subm(r, a, b, L); *rn = an; }
    else { r288241_subm(r, b, a, L); *rn = bn; }
}

static void r288241_mul(uint32_t *r, const uint32_t *a, const uint32_t *b,
                        int L, int F, uint32_t *tmp) {
    for (int i = 0; i < 2 * L; ++i) tmp[i] = 0;
    for (int i = 0; i < L; ++i) {
        if (!a[i]) continue;
        uint64_t carry = 0;
        for (int j = 0; j < L; ++j) {
            uint64_t t = (uint64_t)tmp[i + j] + (uint64_t)a[i] * b[j] + carry;
            tmp[i + j] = (uint32_t)(t % R288241_BASE);
            carry = t / R288241_BASE;
        }
        tmp[i + L] = (uint32_t)carry;
    }
    for (int i = 0; i < L; ++i) r[i] = tmp[F + i];
}

/* set fixed-point number from value scaled by 1e18 (top three limbs) */
static void r288241_set18(uint32_t *d, int L, int F, unsigned __int128 v) {
    for (int i = 0; i < L; ++i) d[i] = 0;
    d[F] = (uint32_t)(v / 1000000000000000000ULL);
    d[F - 1] = (uint32_t)((v / 1000000000ULL) % R288241_BASE);
    d[F - 2] = (uint32_t)(v % R288241_BASE);
}

static unsigned __int128 r288241_top18(const uint32_t *d, int F) {
    return (unsigned __int128)d[F] * 1000000000000000000ULL
         + (unsigned __int128)d[F - 1] * 1000000000ULL + d[F - 2];
}

static char *r288241_dig = NULL;
static idx_t r288241_nd = 0;

static void r288241_compute(idx_t N) {
    int F = (int)(N / 9) + 6;
    int L = F + 1;
    int K = 32 * F + 20;
    uint64_t *c = (uint64_t *)malloc((size_t)(K + 1) * sizeof(uint64_t));
    uint32_t *x = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *ck = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *dl = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *iv = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(2 * L, sizeof(uint32_t));
    uint32_t *u = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *two = (uint32_t *)calloc(L, sizeof(uint32_t));
    for (int k = 0; k <= K; ++k) {
        uint64_t m = (uint64_t)k + 1;
        c[k] = r288241_cbfloor(4ULL * m * m * m);
    }
    /* initial guess x = 0.5134 */
    x[F] = 0; x[F - 1] = 513400000u;
    for (int it = 0; it < 600; ++it) {
        int pn, qn;
        for (int i = 0; i < L; ++i) { p[i] = 0; q[i] = 0; }
        p[F] = (uint32_t)c[K]; pn = 0; qn = 0;
        for (int k = K - 1; k >= 0; --k) {
            r288241_mul(q, q, x, L, F, tmp); qn = !qn;
            r288241_sadd(&qn, q, qn, q, pn, p, L);
            r288241_mul(p, p, x, L, F, tmp); pn = !pn;
            for (int i = 0; i < L; ++i) ck[i] = 0;
            ck[F] = (uint32_t)c[k];
            r288241_sadd(&pn, p, pn, p, 0, ck, L);
        }
        unsigned __int128 qv = r288241_top18(q, F);
        if (qv == 0) break;
        unsigned __int128 inv = (unsigned __int128)1000000000000000000ULL
                              * 1000000000000000000ULL / qv;
        r288241_set18(iv, L, F, inv);
        r288241_mul(dl, p, iv, L, F, tmp);
        int dn = pn ^ qn, xn = 0;
        r288241_sadd(&xn, x, 0, x, dn, dl, L);
        int small = 1;
        for (int i = 2; i < L; ++i) if (dl[i]) { small = 0; break; }
        if (small) break;
    }
    /* reciprocal u = 1/x by Newton */
    {
        unsigned __int128 xv = r288241_top18(x, F);
        unsigned __int128 uv = (unsigned __int128)1000000000000000000ULL
                             * 1000000000000000000ULL / xv;
        r288241_set18(u, L, F, uv);
        two[F] = 2;
        for (int it = 0; it < 14; ++it) {
            r288241_mul(t, x, u, L, F, tmp);
            if (r288241_cmp(two, t, L) < 0) break;
            r288241_subm(t, two, t, L);
            r288241_mul(u, u, t, L, F, tmp);
        }
    }
    free(r288241_dig);
    r288241_dig = (char *)malloc((size_t)N + 1);
    r288241_nd = N;
    static const uint32_t pw[9] = {1, 10, 100, 1000, 10000, 100000,
                                   1000000, 10000000, 100000000};
    r288241_dig[0] = (char)(u[F] % 10);
    for (idx_t j = 1; j < N; ++j) {
        int idx = F - 1 - (int)((j - 1) / 9);
        int pos = (int)((j - 1) % 9);
        r288241_dig[j] = (char)((u[idx] / pw[8 - pos]) % 10);
    }
    free(c); free(x); free(p); free(q); free(ck); free(dl);
    free(iv); free(tmp); free(u); free(t); free(two);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > r288241_nd) {
        idx_t N = n;
        if (N < 2 * r288241_nd) N = 2 * r288241_nd;
        if (N < 120) N = 120;
        r288241_compute(N);
    }
    return (seq_t)(unsigned)r288241_dig[n - 1];
}
