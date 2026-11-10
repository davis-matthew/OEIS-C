/*
 * A378740  Distinct values of A378664(k) in the order of appearance, when k
 *          ranges over those primitively abundant numbers k for which
 *          A378664(k) is less than the largest proper divisor of k.
 * https://oeis.org/A378740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A378664(A378741(n)), the distinct values in order of
 *            first appearance. Computed by a search: a pruned depth-first
 *            enumeration of the primitively abundant numbers k <= L (all
 *            proper divisors non-abundant), with the bound L doubled as
 *            needed. Each k with d = A378664(k) < k/spf(k) is kept, al...
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 65 terms through n = 8954 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

typedef unsigned __int128 S378740_u128;

static uint32_t *S378740_pr = NULL;
static size_t S378740_np = 0;
static uint64_t S378740_L = 0;

static uint64_t S378740_P[64];
static int S378740_E[64];
static uint64_t S378740_S[64];
static size_t S378740_QI[64];
static uint64_t S378740_Q[64];

static uint64_t *S378740_hk = NULL, *S378740_hv = NULL;
static size_t S378740_hcap = 0, S378740_hcnt = 0;

static uint64_t *S378740_vals = NULL;
static size_t S378740_cnt = 0;

static void S378740_hinit(size_t cap)
{
    free(S378740_hk);
    free(S378740_hv);
    S378740_hcap = cap;
    S378740_hcnt = 0;
    S378740_hk = (uint64_t *)calloc(cap, sizeof(uint64_t));
    S378740_hv = (uint64_t *)calloc(cap, sizeof(uint64_t));
    if (!S378740_hk || !S378740_hv) abort();
}

static void S378740_hput_raw(uint64_t d, uint64_t k)
{
    size_t m = S378740_hcap - 1;
    size_t i = (size_t)((d * UINT64_C(0x9E3779B97F4A7C15)) >> 17) & m;
    for (;;) {
        if (S378740_hk[i] == 0) {
            S378740_hk[i] = d;
            S378740_hv[i] = k;
            ++S378740_hcnt;
            return;
        }
        if (S378740_hk[i] == d) {
            if (k < S378740_hv[i]) S378740_hv[i] = k;
            return;
        }
        i = (i + 1) & m;
    }
}

static void S378740_hput(uint64_t d, uint64_t k)
{
    if (2 * (S378740_hcnt + 1) > S378740_hcap) {
        uint64_t *ok = S378740_hk, *ov = S378740_hv;
        size_t oc = S378740_hcap;
        S378740_hk = NULL;
        S378740_hv = NULL;
        S378740_hinit(oc * 2);
        for (size_t i = 0; i < oc; ++i)
            if (ok[i]) S378740_hput_raw(ok[i], ov[i]);
        free(ok);
        free(ov);
    }
    S378740_hput_raw(d, k);
}

static void S378740_div_dfs(int i, int cnt, S378740_u128 d, S378740_u128 s,
                            S378740_u128 f, S378740_u128 *best)
{
    if (i == cnt) {
        S378740_u128 two_d = d + d;
        if (s <= two_d && f > two_d && d > *best) *best = d;
        return;
    }
    uint64_t p = S378740_P[i], q = S378740_Q[i];
    S378740_u128 pp = 1, sp = 1, qq = 1;
    for (int e = 0; e <= S378740_E[i]; ++e) {
        if (e > 0) {
            pp *= p;
            sp += pp;
            qq *= q;
        }
        S378740_div_dfs(i + 1, cnt, d * pp, s * sp, f * qq, best);
    }
}

static void S378740_leaf(int cnt, uint64_t k, S378740_u128 sg)
{
    for (int i = 0; i < cnt; ++i) {
        uint64_t sprev = (S378740_S[i] - 1) / S378740_P[i];
        S378740_u128 sk = sg / S378740_S[i] * sprev;
        if (sk > (S378740_u128)2 * (k / S378740_P[i])) return;
    }
    for (int i = 0; i < cnt; ++i)
        S378740_Q[i] = (S378740_QI[i] + 1 < S378740_np)
                           ? (uint64_t)S378740_pr[S378740_QI[i] + 1]
                           : next_prime(S378740_P[i]);
    S378740_u128 best = 1;
    S378740_div_dfs(0, cnt, 1, 1, 1, &best);
    if (best < (S378740_u128)(k / S378740_P[0]))
        S378740_hput((uint64_t)best, k);
}

/* upper bound for sigma(x)/x over x <= B whose primes are all >= pr[qi] */
static double S378740_F(size_t qi, uint64_t B)
{
    uint64_t prod = 1;
    double f = 1.0;
    for (size_t j = qi;; ++j) {
        if (j >= S378740_np) return 1e18;
        uint64_t p = S378740_pr[j];
        if (prod > B / p) break;
        prod *= p;
        f *= (double)p / (double)(p - 1);
    }
    return f;
}

static void S378740_node(int depth, uint64_t n, S378740_u128 sg, size_t start)
{
    bool perfect = (sg == (S378740_u128)2 * n);
    double r = (double)(uint64_t)sg / (double)n;
    uint64_t B = S378740_L / n;
    for (size_t qi = start; qi < S378740_np; ++qi) {
        uint64_t q = S378740_pr[qi];
        if (q > B) break;
        if (perfect) {
            /* children n*q of a perfect n: for q >= 25 nothing new arises
               (d = 6 for n = 6, d >= k/2 for even perfect n) */
            if (q > 1000) break;
        } else if (r * S378740_F(qi, B) < 2.0 - 1e-9) {
            break;
        }
        uint64_t m = n, pw = 1, spe = 1;
        for (int e = 1; m <= S378740_L / q; ++e) {
            m *= q;
            pw *= q;
            spe += pw;
            S378740_u128 s2 = sg * spe;
            S378740_P[depth] = q;
            S378740_E[depth] = e;
            S378740_S[depth] = spe;
            S378740_QI[depth] = qi;
            if (s2 > (S378740_u128)2 * m) {
                S378740_leaf(depth + 1, m, s2);
                break;
            }
            S378740_node(depth + 1, m, s2, qi + 1);
        }
    }
}

typedef struct { uint64_t k, d; } S378740_pair;

static int S378740_cmp(const void *x, const void *y)
{
    const S378740_pair *a1 = (const S378740_pair *)x;
    const S378740_pair *b1 = (const S378740_pair *)y;
    return (a1->k > b1->k) - (a1->k < b1->k);
}

static void S378740_build(uint64_t L)
{
    S378740_L = L;
    uint64_t s = 1;
    while ((s + 1) * (s + 1) <= 2 * L) ++s;
    uint64_t PL = s + 4096;
    unsigned char *comp = (unsigned char *)calloc((size_t)PL + 1, 1);
    if (!comp) abort();
    free(S378740_pr);
    S378740_pr = (uint32_t *)malloc(((size_t)PL + 1) * sizeof(uint32_t));
    if (!S378740_pr) abort();
    S378740_np = 0;
    for (uint64_t i = 2; i <= PL; ++i) {
        if (!comp[i]) {
            S378740_pr[S378740_np++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= PL; j += i) comp[j] = 1;
        }
    }
    free(comp);

    S378740_hinit(1024);
    S378740_node(0, 1, 1, 0);

    S378740_pair *arr = (S378740_pair *)malloc((S378740_hcnt + 1) * sizeof(S378740_pair));
    if (!arr) abort();
    size_t c = 0;
    for (size_t i = 0; i < S378740_hcap; ++i)
        if (S378740_hk[i]) {
            arr[c].k = S378740_hv[i];
            arr[c].d = S378740_hk[i];
            ++c;
        }
    qsort(arr, c, sizeof(S378740_pair), S378740_cmp);
    free(S378740_vals);
    S378740_vals = (uint64_t *)malloc((c + 1) * sizeof(uint64_t));
    if (!S378740_vals) abort();
    for (size_t i = 0; i < c; ++i) S378740_vals[i] = arr[i].d;
    S378740_cnt = c;
    free(arr);
}

seq_t a(idx_t n)
{
    static uint64_t curL = 0;
    if (n < 1) return (seq_t)0;
    while (S378740_cnt < (size_t)n) {
        uint64_t L = curL ? curL * 2 : ((uint64_t)1 << 20);
        if (L > ((uint64_t)1 << 50)) return (seq_t)0;
        S378740_build(L);
        curL = L;
    }
    return (seq_t)S378740_vals[n - 1];
}
