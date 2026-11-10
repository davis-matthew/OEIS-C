/*
 * A362180  Irregular table read by rows in which the n-th row consists of all
 *          the numbers m such that A323410(m) = n.
 * https://oeis.org/A362180
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   terms of rows k = 2,3,... in order, row k = increasing m with m
 *            - A047994(m) = k (A323410(m) = k), m not a prime power; computed
 *            by a sieve with doubling limit
 * Range:     u8 n <= 78, u16 n <= 4695, u32 n <= 12684, u64 n <= 12684,
 *            u128 n <= 12684, big any n
 * Validated: standard, pass, 78 terms through n = 9779 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *A362180_cache = 0;
static size_t A362180_cnt = 0;

static void A362180_build(uint64_t L)
{
    uint32_t *spf = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
    uint64_t *val = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
    unsigned char *np = (unsigned char *)malloc(L + 1); /* 1 if has >= 2 distinct primes */
    uint64_t i, j, m;
    uint64_t K = 1;
    while ((K + 1) * (K + 1) <= L) K++;
    for (i = 0; i <= L; i++) spf[i] = 0;
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (m = 2; m <= L; m++) {
        uint64_t x = m, phi = 1;
        int d = 0;
        while (x > 1) {
            uint64_t p = spf[x], pe = 1;
            while (x % p == 0) { x /= p; pe *= p; }
            phi *= (pe - 1);
            d++;
        }
        val[m] = m - phi;
        np[m] = (d >= 2);
    }
    size_t *c = (size_t *)malloc((K + 2) * sizeof(size_t));
    for (i = 0; i <= K + 1; i++) c[i] = 0;
    size_t total = 0;
    for (m = 2; m <= L; m++) {
        if (np[m] && val[m] <= K) { c[val[m] + 1]++; total++; }
    }
    for (i = 1; i <= K + 1; i++) c[i] += c[i - 1];
    uint64_t *out = (uint64_t *)malloc((total + 1) * sizeof(uint64_t));
    for (m = 2; m <= L; m++) {
        if (np[m] && val[m] <= K) out[c[val[m]]++] = m;
    }
    free(c); free(spf); free(val); free(np);
    if (A362180_cache) free(A362180_cache);
    A362180_cache = out;
    A362180_cnt = total;
}

seq_t a(idx_t n)
{
    static uint64_t L = 0;
    if (n < 2) return 0;
    size_t idx = (size_t)(n - 2);
    while (idx >= A362180_cnt) {
        if (L == 0) L = 4096; else L *= 2;
        A362180_build(L);
    }
    return (seq_t)A362180_cache[idx];
}
