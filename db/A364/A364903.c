/*
 * A364903  Numbers k such that A060778(k) = 7.
 * https://oeis.org/A364903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with gcd(tau(k), tau(k+1)) = 7 (A060778(k) = 7).
 *            Method: gcd odd forces k or k+1 to be a square m^2; sieve m,
 *            test m^2-1 and m^2 (and m^2, m^2+1)
 * Range:     u8 none, u16 none, u32 n <= 67, u64 n <= 10399, u128 n <= 10399,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 9793 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *A364903_T = 0;
static size_t A364903_cnt = 0, A364903_cap = 0;
static uint64_t A364903_M = 0;

static uint64_t A364903_g(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void A364903_push(uint64_t v)
{
    if (A364903_cnt == A364903_cap) {
        A364903_cap = A364903_cap ? A364903_cap * 2 : 1024;
        A364903_T = (uint64_t *)realloc(A364903_T, A364903_cap * sizeof(uint64_t));
    }
    A364903_T[A364903_cnt++] = v;
}

/* tau of x with the power of 2 removed; *v2 gets the exponent of 2 */
static uint64_t A364903_tau_odd(uint32_t x, const uint32_t *spf, uint64_t *v2)
{
    uint64_t t = 1;
    while (x > 1) {
        uint32_t p = spf[x] ? spf[x] : x;
        uint64_t e = 0;
        while (x % p == 0) { x /= p; e++; }
        if (p == 2) *v2 += e; else t *= (e + 1);
    }
    return t;
}

static uint64_t A364903_tau_trial(uint64_t r)
{
    uint64_t t = 1, e = 0;
    while ((r & 1) == 0) { r >>= 1; e++; }
    t *= (e + 1);
    for (uint64_t i = 3; i * i <= r; i += 2) {
        if (r % i == 0) {
            e = 0;
            while (r % i == 0) { r /= i; e++; }
            t *= (e + 1);
        }
    }
    if (r > 1) t *= 2;
    return t;
}

static void A364903_build(uint64_t M)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)M + 3, sizeof(uint32_t));
    uint64_t L = M + 1;
    for (uint64_t i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            if (i * i <= L)
                for (uint64_t j = i * i; j <= L; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    A364903_cnt = 0;
    for (uint64_t m = 2; m <= M; m++) {
        uint64_t tm = 1;
        uint32_t x = (uint32_t)m;
        while (x > 1) {
            uint32_t p = spf[x] ? spf[x] : x;
            uint64_t e = 0;
            while (x % p == 0) { x /= p; e++; }
            tm *= (2 * e + 1);
        }
        if (tm % 7) continue;
        /* k = m^2 - 1 */
        {
            uint64_t v = 0;
            uint64_t t = A364903_tau_odd((uint32_t)(m - 1), spf, &v);
            t *= A364903_tau_odd((uint32_t)(m + 1), spf, &v);
            t *= (v + 1);
            if (A364903_g(tm, t) == 7) A364903_push(m * m - 1);
        }
        /* k = m^2, neighbor m^2 + 1 */
        {
            uint64_t N = m * m + 1;
            for (uint64_t p = 5; ; p += 4) {
                uint64_t pw = p * p;
                pw = pw * pw * pw;
                if (pw > N) break;
                if (spf[p] != 0) continue;
                if (N % pw == 0) {
                    uint64_t e = 0, r = N;
                    while (r % p == 0) { r /= p; e++; }
                    uint64_t t = (e + 1) * A364903_tau_trial(r);
                    if (A364903_g(tm, t) == 7) A364903_push(m * m);
                    break;
                }
            }
        }
    }
    free(spf);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A364903_M == 0) A364903_M = 1 << 14;
    if (A364903_cnt == 0) A364903_build(A364903_M);
    while ((uint64_t)A364903_cnt < (uint64_t)n) {
        A364903_M *= 2;
        A364903_build(A364903_M);
    }
    return (seq_t)A364903_T[(size_t)(n - 1)];
}
