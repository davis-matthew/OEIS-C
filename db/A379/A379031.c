/*
 * A379031  Odd modified exponential abundant numbers: odd numbers k such that
 *          A241405(k) > 2*k.
 * https://oeis.org/A379031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th odd k with A241405(k) > 2*k; found by a segmented
 *            sieve of the multiplicative A241405 over odd numbers, results
 *            cached
 * Range:     u8 none, u16 n <= 24, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9571 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *g_terms = 0;
static size_t g_cnt = 0, g_cap = 0;
static uint64_t g_next = 1;           /* next odd number to scan */

static uint32_t *g_pr = 0;
static size_t g_prn = 0;
static uint64_t g_plim = 0;

static void A379031_ensure_primes(uint64_t lim)
{
    if (lim <= g_plim) return;
    uint64_t nl = lim * 2 < 1024 ? 1024 : lim * 2;
    char *s = (char *)calloc((size_t)nl + 1, 1);
    free(g_pr);
    g_pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)nl / 2 + 16));
    g_prn = 0;
    for (uint64_t i = 2; i <= nl; i++) {
        if (!s[i]) {
            g_pr[g_prn++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= nl; j += i) s[j] = 1;
        }
    }
    free(s);
    g_plim = nl;
}

static void A379031_scan_block(void)
{
    const uint64_t B = 1u << 16;
    uint64_t L = g_next;
    uint64_t hi = L + 2 * B;
    uint64_t r = 1;
    while (r * r <= hi) r++;
    A379031_ensure_primes(r + 1);
    uint64_t *rem = (uint64_t *)malloc(sizeof(uint64_t) * B);
    uint64_t *prod = (uint64_t *)malloc(sizeof(uint64_t) * B);
    for (uint64_t i = 0; i < B; i++) { rem[i] = L + 2 * i; prod[i] = 1; }
    for (size_t pi = 0; pi < g_prn; pi++) {
        uint64_t p = g_pr[pi];
        if (p == 2) continue;
        if (p * p > hi) break;
        uint64_t m = ((L + p - 1) / p) * p;
        if ((m & 1) == 0) m += p;
        for (; m < hi; m += 2 * p) {
            uint64_t i = (m - L) / 2;
            unsigned e = 0;
            while (rem[i] % p == 0) { rem[i] /= p; e++; }
            uint64_t f = 0, cur = 1;
            for (unsigned d = 1; d <= e + 1; d++) {
                if ((e + 1) % d == 0) f += cur;
                cur *= p;
            }
            prod[i] *= f;
        }
    }
    for (uint64_t i = 0; i < B; i++) {
        if (rem[i] > 1) prod[i] *= (1 + rem[i]);
        uint64_t k = L + 2 * i;
        if (prod[i] > 2 * k) {
            if (g_cnt == g_cap) {
                g_cap = g_cap ? g_cap * 2 : 1024;
                g_terms = (uint64_t *)realloc(g_terms, g_cap * sizeof(uint64_t));
            }
            g_terms[g_cnt++] = k;
        }
    }
    free(rem);
    free(prod);
    g_next = hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (g_cnt < (size_t)n) A379031_scan_block();
    return (seq_t)g_terms[n - 1];
}
