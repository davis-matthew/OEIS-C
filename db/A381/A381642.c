/*
 * A381642  Numbers k such that F(k) > F(m) for all m < k, where F(k) =
 *          A381641(k)/A381639(k).
 * https://oeis.org/A381642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(1) = 1. For k > 1, F(k) =
 *            Sum_{i=1..omega(k)-1} (1 - p_i/p_{i+1}) over the distinct primes
 *            p_1 < ... < p_r of k, and k is a term when F(k) > F(m) for all m
 *            < k. Records up to 1024 come from trial division. After that the
 *            code works through windows (X, 2X]. In each win...
 * Range:     u8 n <= 20, u16 n <= 144, u32 n <= 2160, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 69 terms through n = 2761 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

typedef struct { uint64_t k, num, den; } cand_A381642;

static uint64_t *recs_A381642 = 0;
static size_t cnt_A381642 = 0, cap_A381642 = 0;
static uint64_t lim_A381642 = 0;
static uint64_t bnum_A381642 = 0, bden_A381642 = 1;

static uint32_t *pr_A381642 = 0;
static size_t npr_A381642 = 0;
static unsigned char *isp_A381642 = 0;
static uint64_t prlim_A381642 = 0;

static cand_A381642 *cands_A381642 = 0;
static size_t ccnt_A381642 = 0, ccap_A381642 = 0;
static uint64_t gX_A381642, g2X_A381642;
static double gT_A381642;

#define EPS_A381642 1e-9

static void push_A381642(uint64_t v)
{
    if (cnt_A381642 == cap_A381642) {
        cap_A381642 = cap_A381642 ? cap_A381642 * 2 : 256;
        recs_A381642 = (uint64_t *)realloc(recs_A381642, cap_A381642 * sizeof(uint64_t));
    }
    recs_A381642[cnt_A381642++] = v;
}

static void addcand_A381642(uint64_t k, uint64_t num, uint64_t den)
{
    if (ccnt_A381642 == ccap_A381642) {
        ccap_A381642 = ccap_A381642 ? ccap_A381642 * 2 : 1024;
        cands_A381642 = (cand_A381642 *)realloc(cands_A381642, ccap_A381642 * sizeof(cand_A381642));
    }
    cands_A381642[ccnt_A381642].k = k;
    cands_A381642[ccnt_A381642].num = num;
    cands_A381642[ccnt_A381642].den = den;
    ccnt_A381642++;
}

static void sieve_A381642(uint64_t need)
{
    uint64_t nl = prlim_A381642 * 2;
    if (nl < need) nl = need;
    if (nl < 65536) nl = 65536;
    free(isp_A381642);
    free(pr_A381642);
    isp_A381642 = (unsigned char *)malloc(nl + 1);
    for (uint64_t i = 0; i <= nl; i++) isp_A381642[i] = 1;
    isp_A381642[0] = isp_A381642[1] = 0;
    for (uint64_t i = 2; i * i <= nl; i++)
        if (isp_A381642[i])
            for (uint64_t j = i * i; j <= nl; j += i) isp_A381642[j] = 0;
    size_t c = 0;
    for (uint64_t i = 2; i <= nl; i++) if (isp_A381642[i]) c++;
    pr_A381642 = (uint32_t *)malloc((c + 1) * sizeof(uint32_t));
    c = 0;
    for (uint64_t i = 2; i <= nl; i++) if (isp_A381642[i]) pr_A381642[c++] = (uint32_t)i;
    npr_A381642 = c;
    prlim_A381642 = nl;
}

/* upper bound (from above) for R^(1/s) */
static double rootup_A381642(double R, int s)
{
    if (s == 1) return R;
    double x = 1.0;
    for (;;) {
        double t = 1.0;
        for (int i = 0; i < s; i++) t *= x;
        if (t >= R) break;
        x *= 2.0;
    }
    for (int it = 0; it < 8; it++) {
        double y = 1.0;
        for (int i = 0; i < s - 1; i++) y *= x;
        double xn = ((double)(s - 1) * x + R / y) / (double)s;
        if (xn >= x) break;
        x = xn;
    }
    return x * (1.0 + 1e-12);
}

/* upper bound on F added by >= 1 further primes > p whose product is <= R */
static double ub_A381642(uint64_t p, uint64_t R)
{
    double best = 0.0, pp = (double)p, Rd = (double)R, pw = pp;
    for (int s = 1; s < 64 && pw < Rd * (1.0 + 1e-12); s++, pw *= pp) {
        double r = rootup_A381642(Rd, s);
        double v = (double)s * (1.0 - pp / r);
        if (v > best) best = v;
    }
    return best;
}

static void dfs_A381642(size_t pi, uint64_t p, uint64_t P, uint64_t num, uint64_t den, double f)
{
    uint64_t R = g2X_A381642 / P;
    double T = gT_A381642 - EPS_A381642;
    double d = T - f;
    if (d < 1.0) {
        uint64_t qlo = p + 1, t = gX_A381642 / P + 1;
        if (t > qlo) qlo = t;
        if (d > 0) {
            double th = (double)p / (1.0 - d);
            if (th < 1.8e19) {
                uint64_t qt = (uint64_t)th;
                if (qt > qlo) qlo = qt;
            } else qlo = R + 1;
        }
        if (qlo <= R && !(qlo & 1)) qlo++;
        for (uint64_t q = qlo; q <= R; q += 2) {
            bool ip = q <= prlim_A381642 ? (isp_A381642[q] != 0) : is_prime_u64(q);
            if (!ip) continue;
            addcand_A381642(P * q, num * q + (q - p) * den, den * q);
        }
    }
    for (size_t j = pi + 1; j < npr_A381642; j++) {
        uint64_t q = pr_A381642[j];
        if (q > R / (q + 1)) break;
        uint64_t R2 = R / q;
        double u = ub_A381642(q, R2);
        if (f + 1.0 + u <= T) break;
        double f2 = f + 1.0 - (double)p / (double)q;
        if (f2 + u <= T) continue;
        dfs_A381642(j, q, P * q, num * q + (q - p) * den, den * q, f2);
    }
}

static int cmp_A381642(const void *x, const void *y)
{
    uint64_t a1 = ((const cand_A381642 *)x)->k, b1 = ((const cand_A381642 *)y)->k;
    return a1 < b1 ? -1 : (a1 > b1 ? 1 : 0);
}

static bool better_A381642(uint64_t num, uint64_t den)
{
    return (unsigned __int128)num * bden_A381642 > (unsigned __int128)bnum_A381642 * den;
}

static void seed_A381642(void)
{
    push_A381642(1);
    bnum_A381642 = 0; bden_A381642 = 1;
    for (uint64_t k = 2; k <= 1024; k++) {
        uint64_t ps[16];
        int pc = 0;
        uint64_t t = k;
        for (uint64_t i = 2; i * i <= t; i++)
            if (t % i == 0) { ps[pc++] = i; while (t % i == 0) t /= i; }
        if (t > 1) ps[pc++] = t;
        if (pc < 2) continue;
        uint64_t num = 0, den = 1;
        for (int i = 1; i < pc; i++) {
            num = num * ps[i] + (ps[i] - ps[i - 1]) * den;
            den *= ps[i];
        }
        if (better_A381642(num, den)) {
            bnum_A381642 = num; bden_A381642 = den;
            push_A381642(k);
        }
    }
    lim_A381642 = 1024;
}

seq_t a(idx_t n)
{
    if (cnt_A381642 == 0) seed_A381642();
    while (cnt_A381642 < (size_t)n) {
        uint64_t X = lim_A381642, X2 = 2 * X;
        uint64_t s = 1;
        while (s * s <= X2) s++;
        if (prlim_A381642 < s + 2) sieve_A381642(s + 2);
        gX_A381642 = X;
        g2X_A381642 = X2;
        gT_A381642 = (double)bnum_A381642 / (double)bden_A381642;
        ccnt_A381642 = 0;
        for (size_t j = 0; j < npr_A381642; j++) {
            uint64_t p1 = pr_A381642[j];
            if (p1 > X2 / (p1 + 1)) break;
            double u = ub_A381642(p1, X2 / p1);
            if (u <= gT_A381642 - EPS_A381642) break;
            dfs_A381642(j, p1, p1, 0, 1, 0.0);
        }
        if (ccnt_A381642 > 1)
            qsort(cands_A381642, ccnt_A381642, sizeof(cand_A381642), cmp_A381642);
        for (size_t i = 0; i < ccnt_A381642; i++) {
            if (better_A381642(cands_A381642[i].num, cands_A381642[i].den)) {
                bnum_A381642 = cands_A381642[i].num;
                bden_A381642 = cands_A381642[i].den;
                push_A381642(cands_A381642[i].k);
            }
        }
        lim_A381642 = X2;
    }
    return (seq_t)recs_A381642[(size_t)n - 1];
}
