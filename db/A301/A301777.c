/*
 * A301777  Expansion of Product_{k>=1} (1 + x^k)^A001001(k).
 * https://oeis.org/A301777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{k>=1} (1 + x^k)^A001001(k); the series is built
 *            by multiplying in each factor (1+x^k)^e = Sum_{j>=0}
 *            binomial(e,j)*x^(k*j) (no division, so every intermediate value
 *            is <= a(n))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 24, u64 n <= 55, u128 n <= 133,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 952 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t A001001_u64(uint64_t n) {
    if (n == 1) return 1;
    uint64_t result = 1, temp = n;
    for (uint64_t p = 2; p * p <= temp; p++) {
        int e = 0;
        while (temp % p == 0) { e++; temp /= p; }
        if (e > 0) {
            uint64_t s1 = 0, pp = 1;
            for (int i = 0; i <= e; i++) { s1 += pp; pp *= p; }
            uint64_t s2 = s1 + pp;
            result *= (s1 * s2) / (p + 1);
        }
    }
    if (temp > 1) result *= temp * temp + temp + 1;
    return result;
}

static uint64_t A301777_gcd64(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static seq_t *g_av = 0;
static idx_t g_cap = 0;

seq_t a(idx_t n) {
    if (g_av == 0 || n >= g_cap) {
        idx_t m = n + 1;
        if (m < 64) m = 64;
        if (g_av) { mem_free(g_av); g_av = 0; }
        seq_t *P = mem_new(seq_t, (size_t)m + 1);
        seq_t *c = mem_new(seq_t, (size_t)m + 1);
        uint64_t *num = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)m + 1));
        P[0] = 1;
        for (idx_t k = 1; k < m; k++) {
            uint64_t e = A001001_u64(k);
            idx_t J = (m - 1) / k;
            c[0] = 1;
            for (idx_t j = 1; j <= J; j++) {
                num[j - 1] = (e >= j - 1) ? e - (j - 1) : 0;
                uint64_t rem = j;
                for (idx_t t = 0; t < j && rem > 1; t++) {
                    uint64_t g = A301777_gcd64(num[t], rem);
                    if (g > 1) { num[t] /= g; rem /= g; }
                }
                seq_t prod = 1;
                for (idx_t t = 0; t < j; t++) prod *= (seq_t)num[t];
                c[j] = prod;
            }
            for (idx_t i = m - 1; i >= k; i--) {
                seq_t s = P[i];
                for (idx_t j = 1; j * k <= i; j++)
                    s += c[j] * P[i - j * k];
                P[i] = s;
            }
        }
        free(num);
        mem_free(c);
        g_av = P;
        g_cap = m;
    }
    return g_av[n];
}
