/*
 * A373259  Numbers k such that k' / gcd(k,k') is a multiple of 4 and
 *          A276085(k) is a multiple of 8, where k' stands for the arithmetic
 *          derivative of k, A003415, and A276085 is the primorial base
 *          log-function.
 * https://oeis.org/A373259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with (k'/gcd(k,k')) == 0 (mod 4) and
 *            A276085(k) == 0 (mod 8); found by sieving smallest prime factors
 *            and scanning k, doubling the limit as needed
 * Range:     u8 n <= 19, u16 n <= 5454, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9024 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint64_t *s_terms = NULL;
static size_t s_count = 0;

static uint64_t gcd_local_u64(uint64_t x, uint64_t y)
{
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static void rebuild_terms(uint64_t limit)
{
    uint32_t *spf = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    unsigned char *pm = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t i, j;
    for (i = 0; i <= limit; ++i) {
        spf[i] = 0;
        pm[i] = 0;
    }
    for (i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            for (j = i; j <= limit; j += i)
                if (spf[j] == 0)
                    spf[j] = (uint32_t)i;
        }
    }
    unsigned prod = 1;
    for (i = 2; i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {
            pm[i] = (unsigned char)(prod & 7);
            prod = (prod * (unsigned)(i & 7)) & 7;
        }
    }
    free(s_terms);
    s_terms = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    s_count = 0;
    for (uint64_t k = 1; k <= limit; ++k) {
        uint64_t d = 0;
        uint64_t s = 0;
        uint64_t m = k;
        while (m > 1) {
            uint64_t p = spf[m];
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            d += (k / p) * e;
            s += e * pm[p];
        }
        uint64_t g = gcd_local_u64(k, d);
        uint64_t q = d / g;
        if ((q & 3) == 0 && (s & 7) == 0)
            s_terms[s_count++] = k;
    }
    free(spf);
    free(pm);
}

seq_t a(idx_t n)
{
    static uint64_t limit = 0;
    if (n < 1)
        return (seq_t)0;
    while (s_count < (size_t)n) {
        limit = limit ? limit * 2 : ((uint64_t)1 << 16);
        rebuild_terms(limit);
    }
    return (seq_t)s_terms[(size_t)(n - 1)];
}
