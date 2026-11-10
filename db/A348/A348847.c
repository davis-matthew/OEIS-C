/*
 * A348847  Numbers k where the ratios A099378(k)/k reach a record value.
 * https://oeis.org/A348847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search, conjecture-based. Every k <= 1000 is tested directly.
 *            Above 1000 only squares m^2 and twice-squares 2*m^2 are tested:
 *            for any other k, sigma(k) and tau(k) are both even, so
 *            A099378(k)/k <= sigma(k)/(2k). a(n) = the n-th k where
 *            A099378(k)/k = sigma(k)/(k*gcd(k*tau(k), sigma(k))) reaches a...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

typedef unsigned __int128 A348847_u128;

static uint64_t *A348847_rec = NULL;
static size_t A348847_cnt = 0, A348847_cap = 0;
static A348847_u128 A348847_bestA = 0;
static uint64_t A348847_bestK = 0;
static int A348847_init = 0;
static uint64_t A348847_ms = 32, A348847_mt = 23;

static uint32_t *A348847_spf = NULL;
static uint64_t A348847_spfN = 0;

static A348847_u128 A348847_gcd(A348847_u128 x, A348847_u128 y)
{
    while (y) { A348847_u128 t = x % y; x = y; y = t; }
    return x;
}

static void A348847_ensure_spf(uint64_t m)
{
    if (m <= A348847_spfN) return;
    uint64_t N = A348847_spfN * 2;
    if (N < m) N = m;
    if (N < 1024) N = 1024;
    free(A348847_spf);
    A348847_spf = (uint32_t *)calloc((size_t)(N + 1), sizeof(uint32_t));
    if (!A348847_spf) abort();
    for (uint64_t i = 2; i <= N; ++i) {
        if (A348847_spf[i] == 0) {
            for (uint64_t j = i; j <= N; j += i)
                if (A348847_spf[j] == 0) A348847_spf[j] = (uint32_t)i;
        }
    }
    A348847_spfN = N;
}

static void A348847_push(uint64_t k)
{
    if (A348847_cnt >= A348847_cap) {
        A348847_cap = A348847_cap ? A348847_cap * 2 : 256;
        A348847_rec = (uint64_t *)realloc(A348847_rec, sizeof(uint64_t) * A348847_cap);
        if (!A348847_rec) abort();
    }
    A348847_rec[A348847_cnt++] = k;
}

/* evaluate k given its factorization */
static void A348847_eval(uint64_t k, const uint64_t *pr, const uint64_t *ex, int cnt)
{
    A348847_u128 sig = 1, tau = 1;
    for (int i = 0; i < cnt; ++i) {
        A348847_u128 pp = 1;
        for (uint64_t e = 0; e <= ex[i]; ++e) pp *= pr[i];
        sig *= (pp - 1) / (pr[i] - 1);
        tau *= (ex[i] + 1);
    }
    A348847_u128 g = A348847_gcd((A348847_u128)k * tau, sig);
    A348847_u128 A = sig / g;
    if (A348847_cnt == 0 ||
        A * (A348847_u128)A348847_bestK > A348847_bestA * (A348847_u128)k) {
        A348847_bestA = A;
        A348847_bestK = k;
        A348847_push(k);
    }
}

static void A348847_small(uint64_t k)
{
    uint64_t pr[64], ex[64];
    int cnt = 0;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            pr[cnt] = p; ex[cnt] = e; ++cnt;
        }
    }
    if (m > 1) { pr[cnt] = m; ex[cnt] = 1; ++cnt; }
    A348847_eval(k, pr, ex, cnt);
}

static void A348847_square(uint64_t m, int c)
{
    uint64_t pr[64], ex[64];
    int cnt = 0;
    A348847_ensure_spf(m);
    uint64_t x = m;
    int has2 = 0;
    while (x > 1) {
        uint64_t p = A348847_spf[x];
        uint64_t e = 0;
        while (x % p == 0) { x /= p; ++e; }
        pr[cnt] = p; ex[cnt] = 2 * e;
        if (p == 2 && c == 2) { ex[cnt] += 1; has2 = 1; }
        ++cnt;
    }
    if (c == 2 && !has2) {
        for (int i = cnt; i > 0; --i) { pr[i] = pr[i - 1]; ex[i] = ex[i - 1]; }
        pr[0] = 2; ex[0] = 1; ++cnt;
    }
    A348847_eval((uint64_t)c * m * m, pr, ex, cnt);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A348847_init) {
        A348847_init = 1;
        for (uint64_t k = 1; k <= 1000; ++k) A348847_small(k);
    }
    while ((idx_t)A348847_cnt < n) {
        uint64_t ks = A348847_ms * A348847_ms;
        uint64_t kt = 2 * A348847_mt * A348847_mt;
        if (ks < kt) { A348847_square(A348847_ms, 1); ++A348847_ms; }
        else         { A348847_square(A348847_mt, 2); ++A348847_mt; }
    }
    return (seq_t)A348847_rec[n - 1];
}
