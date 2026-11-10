/*
 * A344732  Numbers k such that k divides Sum_{j=1..k} A048105(j).
 * https://oeis.org/A344732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. Sum_{j=1..k} A048105(j) = D(k) - Sum_{m>=1}
 *            mu(m)*D(floor(k/m^2)), where D(y) = Sum_{i<=y} d(i). For each
 *            quotient q, locate where the sum is about q*k, then sieve a
 *            window around that point and keep every k with k | Sum_{j=1..k}
 *            A048105(j)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 22, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define A344732_MUMAX 520000

static int8_t *A344732_mu = 0;
static uint32_t *A344732_pr = 0;
static size_t A344732_np = 0;

static void A344732_init(void)
{
    if (A344732_mu) return;
    size_t N = A344732_MUMAX;
    A344732_mu = (int8_t *)malloc(N + 1);
    char *comp = (char *)calloc(N + 1, 1);
    A344732_pr = (uint32_t *)malloc(sizeof(uint32_t) * 50000);
    for (size_t i = 0; i <= N; i++) A344732_mu[i] = 1;
    for (size_t i = 2; i <= N; i++) {
        if (!comp[i]) {
            A344732_pr[A344732_np++] = (uint32_t)i;
            for (size_t j = i; j <= N; j += i) {
                if (j > i) comp[j] = 1;
                A344732_mu[j] = (int8_t)(-A344732_mu[j]);
            }
            if (i <= N / i)
                for (size_t j = i * i; j <= N; j += i * i) A344732_mu[j] = 0;
        }
    }
    free(comp);
}

static uint64_t A344732_isq(uint64_t x)
{
    uint64_t r = 0, b = (uint64_t)1 << 62;
    while (b > x) b >>= 2;
    while (b) {
        if (x >= r + b) { x -= r + b; r = (r >> 1) + b; }
        else r >>= 1;
        b >>= 2;
    }
    return r;
}

static uint64_t A344732_D(uint64_t y)
{
    if (y == 0) return 0;
    uint64_t r = A344732_isq(y), s = 0;
    for (uint64_t i = 1; i <= r; i++) s += y / i;
    return 2 * s - r * r;
}

/* prefix sum of A048105 */
static uint64_t A344732_S(uint64_t x)
{
    if (x == 0) return 0;
    uint64_t pos = 0, neg = 0;
    for (uint64_t m = 1; m * m <= x; m++) {
        int8_t u = A344732_mu[m];
        if (u == 0) continue;
        uint64_t v = A344732_D(x / (m * m));
        if (u > 0) pos += v; else neg += v;
    }
    return A344732_D(x) - (pos - neg);
}

static long long A344732_f(uint64_t k, uint64_t q)
{
    return (long long)A344732_S(k) - (long long)(q * k);
}

static uint64_t A344732_terms[64];
static int A344732_nt = 0;
static uint64_t A344732_q = 0;
static uint64_t A344732_prevhi = 0;
static int A344732_done = 0;

static void A344732_window(uint64_t lo, uint64_t hi)
{
    uint64_t s = A344732_S(lo - 1);
    const uint64_t BL = (uint64_t)1 << 20;
    uint64_t *rem = (uint64_t *)malloc(sizeof(uint64_t) * BL);
    uint32_t *dc = (uint32_t *)malloc(sizeof(uint32_t) * BL);
    uint8_t *om = (uint8_t *)malloc(BL);
    for (uint64_t b = lo; b <= hi; b += BL) {
        uint64_t e = b + BL - 1;
        if (e > hi) e = hi;
        uint64_t len = e - b + 1;
        for (uint64_t j = 0; j < len; j++) { rem[j] = b + j; dc[j] = 1; om[j] = 0; }
        uint64_t sq = A344732_isq(e);
        for (size_t pi = 0; pi < A344732_np; pi++) {
            uint64_t p = A344732_pr[pi];
            if (p > sq) break;
            uint64_t first = ((b + p - 1) / p) * p;
            for (uint64_t v = first; v <= e; v += p) {
                uint64_t j = v - b, r = rem[j];
                uint32_t ex = 0;
                while (r % p == 0) { r /= p; ex++; }
                rem[j] = r;
                dc[j] *= ex + 1;
                om[j]++;
            }
        }
        for (uint64_t j = 0; j < len; j++) {
            uint32_t d = dc[j];
            uint32_t w = om[j];
            if (rem[j] > 1) { d *= 2; w++; }
            s += (uint64_t)d - ((uint64_t)1 << w);
            uint64_t k = b + j;
            if (s % k == 0 && k > A344732_prevhi && A344732_nt < 64)
                A344732_terms[A344732_nt++] = k;
        }
    }
    free(rem); free(dc); free(om);
}

static void A344732_next_q(void)
{
    uint64_t q = A344732_q++;
    if (q > 40) { A344732_done = 1; return; }
    uint64_t lo = 0, hi = 1;
    while (A344732_f(hi, q) < 0) {
        lo = hi;
        hi *= 2;
        if (hi > 200000000000ULL) { A344732_done = 1; return; }
    }
    while (hi - lo > lo / 1024 + 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (A344732_f(mid, q) >= 0) hi = mid; else lo = mid;
    }
    long long k = (long long)hi;
    for (int it = 0; it < 8; it++) {
        long long f = A344732_f((uint64_t)k, q);
        long long den = 392 + f * 1000 / k;
        if (den <= 0) break;
        long long step = f * 1000 / den;
        if (step > -3 && step < 3) break;
        k -= step;
        if (k < 1) { k = 1; break; }
    }
    uint64_t kk = (uint64_t)k;
    uint64_t W = 8 * A344732_isq(kk) + 64;
    uint64_t wl = kk > W ? kk - W : 1;
    uint64_t wh = kk + W;
    if (wl <= A344732_prevhi) wl = A344732_prevhi + 1;
    if (wl <= wh) {
        A344732_window(wl, wh);
        A344732_prevhi = wh;
    }
}

seq_t a(idx_t n)
{
    A344732_init();
    if (n == 0) return (seq_t)0;
    while ((idx_t)A344732_nt < n && !A344732_done) A344732_next_q();
    if ((idx_t)A344732_nt < n) return (seq_t)0;
    return (seq_t)A344732_terms[n - 1];
}
