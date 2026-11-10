/*
 * A337146  Numbers k such that abs(A337145(k))/8 is prime.
 * https://oeis.org/A337146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that |A337145(k)|/8 is prime, where
 *            A337145(k) = p1(k)*p7(k) - p3(k)*p5(k), p_r(k) = k-th prime == r
 *            mod 8; computed by sieve + search
 * Range:     u8 n <= 19, u16 n <= 2187, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9774 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

static uint64_t *g_terms = NULL;
static size_t g_tc = 0;
static uint64_t g_limit = 0;

static uint64_t mm_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

static uint64_t mm_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = mm_mulmod(r, b, m);
        b = mm_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool mm_is_prime(uint64_t n)
{
    static const uint64_t bs[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    if (n < 2) return false;
    for (int i = 0; i < 12; i++) {
        if (n % bs[i] == 0) return n == bs[i];
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = mm_powmod(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = mm_mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static void rebuild(uint64_t limit)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)limit + 1, 1);
    size_t cap = (size_t)limit / 4 + 16;
    uint64_t *pr[4];
    size_t cnt[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++) pr[i] = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (uint64_t i = 2; i <= limit; i++) {
        if (!comp[i]) {
            if (i * i <= limit)
                for (uint64_t j = i * i; j <= limit; j += i) comp[j] = 1;
            int r = (int)(i & 7);
            if (r == 1) pr[0][cnt[0]++] = i;
            else if (r == 3) pr[1][cnt[1]++] = i;
            else if (r == 5) pr[2][cnt[2]++] = i;
            else if (r == 7) pr[3][cnt[3]++] = i;
        }
    }
    size_t kmax = cnt[0];
    for (int i = 1; i < 4; i++) if (cnt[i] < kmax) kmax = cnt[i];
    free(g_terms);
    g_terms = (uint64_t *)malloc((kmax + 1) * sizeof(uint64_t));
    g_tc = 0;
    for (size_t k = 0; k < kmax; k++) {
        int64_t det = (int64_t)(pr[0][k] * pr[3][k]) - (int64_t)(pr[1][k] * pr[2][k]);
        uint64_t ad = det < 0 ? (uint64_t)(-det) : (uint64_t)det;
        if (ad % 8 == 0 && mm_is_prime(ad / 8))
            g_terms[g_tc++] = (uint64_t)(k + 1);
    }
    for (int i = 0; i < 4; i++) free(pr[i]);
    free(comp);
    g_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (g_limit == 0) rebuild((uint64_t)1 << 16);
    while ((uint64_t)n > g_tc) rebuild(g_limit * 2);
    return (seq_t)g_terms[n - 1];
}
