/*
 * A349284  Numbers k such that A051378(k) > 2*k and A333926(k) <= 2*k.
 * https://oeis.org/A349284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. The (1+e)-divisor sum and the recursive-divisor sum
 *            differ only when some prime exponent e >= 8, so enumerate k =
 *            p^8 * j up to a bound, compute both multiplicative sums, and
 *            keep the k with A051378(k) > 2k >= A333926(k). Each k is counted
 *            once, under its smallest prime with exponent >= 8...
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 47 terms through n = 9932 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define A349284_MAXE 64

/* rmarks[k][d] == true  <=>  d is a recursive divisor of k (1 <= d <= k) */
static bool A349284_rmarks[A349284_MAXE + 1][A349284_MAXE + 1];
static bool A349284_ready = false;

static void A349284_init(void)
{
    if (A349284_ready) return;
    for (int k = 1; k <= A349284_MAXE; ++k) {
        for (int d = 1; d <= k; ++d) {
            if (k % d != 0) { A349284_rmarks[k][d] = false; continue; }
            bool ok = true;
            int m = d;
            for (int p = 2; m > 1 && ok; ++p) {
                if (m % p != 0) continue;
                int ea = 0, eb = 0, t = k;
                while (m % p == 0) { m /= p; ++ea; }
                while (t % p == 0) { t /= p; ++eb; }
                /* ea <= eb < k, so this entry is already computed */
                if (!A349284_rmarks[eb][ea]) ok = false;
            }
            A349284_rmarks[k][d] = ok;
        }
    }
    A349284_ready = true;
}

static uint64_t *A349284_terms = NULL;
static size_t A349284_cnt = 0;
static uint64_t A349284_X = 0;

static int A349284_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void A349284_pp(uint64_t q, int e, uint64_t *t1, uint64_t *t2)
{
    uint64_t pw = 1, s1 = 1, s2 = 1;
    for (int d = 1; d <= e; ++d) {
        pw *= q;
        if (e % d == 0) s1 += pw;
        if (A349284_rmarks[e][d]) s2 += pw;
    }
    *t1 = s1; *t2 = s2;
}

static void A349284_build(uint64_t X)
{
    uint64_t M = X / 256 + 1;
    uint32_t *spf = (uint32_t *)calloc((size_t)M + 1, sizeof(uint32_t));
    for (uint64_t i = 2; i <= M; ++i)
        if (spf[i] == 0)
            for (uint64_t j = i; j <= M; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;

    size_t cap = 1024, cnt = 0;
    uint64_t *res = (uint64_t *)malloc(cap * sizeof(uint64_t));

    for (uint64_t p = 2; p <= M; ++p) {
        if (spf[p] != p) continue;
        uint64_t p8 = 1;
        for (int i = 0; i < 8; ++i) p8 *= p;
        if (p8 > X) break;
        uint64_t Mp = X / p8;
        for (uint64_t j = 1; j <= Mp; ++j) {
            uint64_t k = p8 * j;
            uint64_t m = j, s1 = 1, s2 = 1;
            int ep = 8;
            bool skip = false;
            while (m > 1) {
                uint64_t q = spf[m];
                int e = 0;
                while (m % q == 0) { m /= q; ++e; }
                if (q == p) { ep += e; continue; }
                if (q < p && e >= 8) { skip = true; break; }
                uint64_t t1, t2;
                A349284_pp(q, e, &t1, &t2);
                s1 *= t1; s2 *= t2;
            }
            if (skip) continue;
            uint64_t t1, t2;
            A349284_pp(p, ep, &t1, &t2);
            s1 *= t1; s2 *= t2;
            if (s1 > 2 * k && s2 <= 2 * k) {
                if (cnt == cap) {
                    cap *= 2;
                    res = (uint64_t *)realloc(res, cap * sizeof(uint64_t));
                }
                res[cnt++] = k;
            }
        }
    }
    free(spf);
    qsort(res, cnt, sizeof(uint64_t), A349284_cmp);
    free(A349284_terms);
    A349284_terms = res;
    A349284_cnt = cnt;
    A349284_X = X;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    A349284_init();
    while (A349284_cnt < (size_t)n)
        A349284_build(A349284_X ? A349284_X * 2 : ((uint64_t)1 << 22));
    return (seq_t)A349284_terms[n - 1];
}
