/*
 * A388276  Numbers k for which A053585(sigma(k)) is equal to A053585(k) and
 *          that satisfy also Euler's condition for odd perfect numbers
 *          (A228058).
 * https://oeis.org/A388276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. k is odd with k = p^e * m^2, where p == 1 (mod 4) is
 *            prime, e == 1 (mod 4), m > 1 and gcd(p,m) = 1 (Euler form,
 *            A228058), and A053585(sigma(k)) = A053585(k). The search
 *            enumerates m and gets p from factoring sigma(m^2). If p is
 *            larger than every prime factor of m, then p^e must be the lar...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 197 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *g_terms = 0;
static size_t g_cnt = 0;
static uint64_t g_N = 0;

static uint32_t *g_pr = 0;
static size_t g_npr = 0;
static uint32_t *g_spf = 0;
static uint64_t g_spf_lim = 0;

static uint64_t A388276_isqrt(uint64_t x)
{
    if (x < 2) return x;
    uint64_t r = x, y = (x >> 1) + 1;
    if (y < r) r = y;
    while (1) {
        uint64_t nr = (r + x / r) >> 1;
        if (nr >= r) break;
        r = nr;
    }
    while (r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

/* largest prime factor of x and its exponent, by trial division */
static void A388276_lpe(uint64_t x, uint64_t *lp, unsigned *ex)
{
    *lp = 1; *ex = 0;
    for (size_t i = 0; i < g_npr; i++) {
        uint64_t p = g_pr[i];
        if (p * p > x) break;
        if (x % p == 0) {
            unsigned e = 0;
            while (x % p == 0) { x /= p; e++; }
            *lp = p; *ex = e;
        }
    }
    if (x > 1) { *lp = x; *ex = 1; }
}

/* same via spf table (x <= g_spf_lim) */
static void A388276_lpe_spf(uint64_t x, uint64_t *lp, unsigned *ex)
{
    *lp = 1; *ex = 0;
    while (x > 1) {
        uint64_t q = g_spf[x];
        unsigned e = 0;
        while (x % q == 0) { x /= q; e++; }
        *lp = q; *ex = e;
    }
}

static int A388276_cmp(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

static void A388276_push(uint64_t **arr, size_t *cnt, size_t *cap, uint64_t v)
{
    if (*cnt == *cap) {
        *cap = *cap ? *cap * 2 : 256;
        *arr = (uint64_t *)realloc(*arr, sizeof(uint64_t) * (*cap));
    }
    (*arr)[(*cnt)++] = v;
}

static void A388276_compute(uint64_t N)
{
    uint64_t M = A388276_isqrt(N / 5);
    uint64_t L = A388276_isqrt(2 * N) + 2;
    if (L < M + 3) L = M + 3;

    /* primes up to L */
    free(g_pr);
    char *s = (char *)calloc(L + 1, 1);
    g_pr = (uint32_t *)malloc(sizeof(uint32_t) * (L / 2 + 10));
    g_npr = 0;
    for (uint64_t i = 2; i <= L; i++) {
        if (!s[i]) {
            g_pr[g_npr++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= L; j += i) s[j] = 1;
        }
    }
    free(s);

    /* spf up to M+2 */
    free(g_spf);
    g_spf_lim = M + 2;
    g_spf = (uint32_t *)calloc(g_spf_lim + 1, sizeof(uint32_t));
    for (uint64_t i = 2; i <= g_spf_lim; i++) {
        if (g_spf[i] == 0) {
            for (uint64_t j = i; j <= g_spf_lim; j += i)
                if (g_spf[j] == 0) g_spf[j] = (uint32_t)i;
        }
    }

    uint64_t *res = 0;
    size_t cnt = 0, cap = 0;

    for (uint64_t m = 3; m <= M; m += 2) {
        uint64_t mm = m * m;
        if (mm > N / 5) break;
        uint64_t rem = N / mm;

        /* factor m, compute (lp1,e1) of sigma(m^2) */
        uint64_t x = m, r = 1, lp1 = 1;
        unsigned f = 0, e1 = 0;
        while (x > 1) {
            uint64_t q = g_spf[x];
            unsigned a_ = 0;
            while (x % q == 0) { x /= q; a_++; }
            r = q; f = a_;
            uint64_t sm = 1, pw = 1;
            for (unsigned j = 0; j < 2 * a_; j++) { pw *= q; sm += pw; }
            uint64_t l; unsigned e;
            A388276_lpe(sm, &l, &e);
            if (l > lp1) { lp1 = l; e1 = e; }
            else if (l == lp1 && l > 1) e1 += e;
        }

        /* Case B: special prime p is the largest prime of k */
        if (lp1 > r && lp1 % 4 == 1 && e1 % 4 == 1) {
            uint64_t p = lp1, pw = 1;
            bool ok = true;
            for (unsigned j = 0; j < e1; j++) {
                if (pw > rem / p) { ok = false; break; }
                pw *= p;
            }
            if (ok) {
                bool good = true;
                if (e1 > 1) {
                    uint64_t sg = 1, t = 1;
                    for (unsigned j = 0; j < e1; j++) { t *= p; sg += t; }
                    uint64_t l2; unsigned ee;
                    A388276_lpe(sg, &l2, &ee);
                    if (l2 >= p) good = false;
                }
                if (good) A388276_push(&res, &cnt, &cap, mm * pw);
            }
        }

        /* Case A: special prime p < r, largest prime power is r^(2f) */
        if (lp1 <= r) {
            for (size_t i = 0; i < g_npr; i++) {
                uint64_t p = g_pr[i];
                if (p >= r || p > rem) break;
                if (p % 4 != 1 || m % p == 0) continue;
                uint64_t p4 = p * p * p * p;
                uint64_t pw = p;
                unsigned e = 1;
                while (1) {
                    uint64_t l2; unsigned e2;
                    if (e == 1) A388276_lpe_spf(p + 1, &l2, &e2);
                    else {
                        uint64_t sg = 1, t = 1;
                        for (unsigned j = 0; j < e; j++) { t *= p; sg += t; }
                        A388276_lpe(sg, &l2, &e2);
                    }
                    uint64_t LL = lp1 > l2 ? lp1 : l2;
                    unsigned EE = (lp1 == LL ? e1 : 0) + (l2 == LL ? e2 : 0);
                    if (LL == r && EE == 2 * f)
                        A388276_push(&res, &cnt, &cap, mm * pw);
                    if (pw > rem / p4) break;
                    pw *= p4;
                    e += 4;
                }
            }
        }
    }

    if (cnt) qsort(res, cnt, sizeof(uint64_t), A388276_cmp);
    free(g_terms);
    g_terms = res;
    g_cnt = cnt;
    g_N = N;
}

seq_t a(idx_t n)
{
    while (g_N == 0 || g_cnt < (size_t)n) {
        uint64_t N = g_N ? g_N * 4 : ((uint64_t)1 << 22);
        A388276_compute(N);
    }
    return (seq_t)g_terms[n - 1];
}
