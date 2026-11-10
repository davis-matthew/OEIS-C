/*
 * A323405  Lexicographically earliest sequence such that for all i, j, a(i) =
 *          a(j) => f(i) = f(j) where f(n) = [A003557(n), A023900(n),
 *          A063994(n)] for all other numbers, except f(n) = 0 for odd primes.
 * https://oeis.org/A323405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted-growth label of f(n) = [A003557(n),
 *            A023900(n), A063994(n)] (0 for odd primes), numbered by first
 *            occurrence; computed by a sieve plus hashing of the triples
 * Range:     u8 n <= 331, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 117 terms through n = 8927 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A323405_lab = NULL;
static uint64_t A323405_cap = 0;

static uint64_t A323405_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t A323405_mix(uint64_t x, uint64_t y, uint64_t z) {
    uint64_t h = x * 0x9E3779B97F4A7C15ULL;
    h ^= (h >> 29);
    h += y * 0xC2B2AE3D27D4EB4FULL;
    h ^= (h >> 31);
    h += z * 0x165667B19E3779F9ULL;
    h ^= (h >> 32);
    h *= 0x9E3779B97F4A7C15ULL;
    h ^= (h >> 28);
    return h;
}

static void A323405_build(uint64_t N) {
    uint32_t *spf = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    uint32_t *lab = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    uint64_t *t1 = (uint64_t *)malloc((N + 2) * sizeof(uint64_t));
    int64_t *t2 = (int64_t *)malloc((N + 2) * sizeof(int64_t));
    uint64_t *t3 = (uint64_t *)malloc((N + 2) * sizeof(uint64_t));
    uint64_t hs = 1;
    while (hs < 2 * (N + 2)) hs <<= 1;
    uint32_t *ht = (uint32_t *)calloc(hs, sizeof(uint32_t));
    uint64_t i, j;
    for (i = 0; i <= N; i++) spf[i] = 0;
    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    uint32_t classes = 0;
    lab[0] = 0;
    for (uint64_t k = 1; k <= N; k++) {
        uint64_t x1, x3;
        int64_t x2;
        if (k > 2 && spf[k] == k) {
            x1 = 0; x2 = 0; x3 = 0;
        } else {
            uint64_t m = k, rad = 1;
            x2 = 1; x3 = 1;
            while (m > 1) {
                uint64_t p = spf[m];
                while (m % p == 0) m /= p;
                rad *= p;
                x2 *= (int64_t)1 - (int64_t)p;
                if (k > 1) x3 *= A323405_gcd(p - 1, k - 1);
            }
            x1 = k / rad;
        }
        uint64_t h = A323405_mix(x1, (uint64_t)x2, x3) & (hs - 1);
        while (ht[h]) {
            uint32_t c = ht[h];
            if (t1[c] == x1 && t2[c] == x2 && t3[c] == x3) break;
            h = (h + 1) & (hs - 1);
        }
        if (!ht[h]) {
            classes++;
            t1[classes] = x1; t2[classes] = x2; t3[classes] = x3;
            ht[h] = classes;
        }
        lab[k] = ht[h];
    }
    free(spf); free(t1); free(t2); free(t3); free(ht);
    free(A323405_lab);
    A323405_lab = lab;
    A323405_cap = N;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    if (m > A323405_cap) {
        uint64_t N = m;
        if (N < 2 * A323405_cap && 2 * A323405_cap <= 20000000ULL) N = 2 * A323405_cap;
        if (N < 1024) N = 1024;
        A323405_build(N);
    }
    return (seq_t)A323405_lab[m];
}
