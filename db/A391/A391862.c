/*
 * A391862  Numbers k that are coprime to A276085(k), where A276085 is the
 *          primorial base log-function.
 * https://oeis.org/A391862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with gcd(k, A276085(k)) = 1, where A276085 is
 *            fully additive with A276085(p) = (product of primes < p); found
 *            by search, using A276085(k) mod k
 * Range:     u8 n <= 183, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 8965 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t *g_terms = NULL;
static size_t g_nterms = 0, g_cap = 0;
static uint64_t g_nextk = 1;

static uint32_t *g_pr = NULL;
static size_t g_npr = 0;
static uint64_t g_lim = 0;

static void ensure_primes(uint64_t lim)
{
    if (lim <= g_lim) return;
    uint64_t L = g_lim ? g_lim : 1024;
    while (L < lim) L *= 2;
    char *c = (char *)calloc((size_t)L + 1, 1);
    free(g_pr);
    g_pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)L / 2 + 2));
    g_npr = 0;
    for (uint64_t i = 2; i <= L; ++i) {
        if (!c[i]) {
            g_pr[g_npr++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= L; j += i) c[j] = 1;
        }
    }
    free(c);
    g_lim = L;
}

/* true if gcd(k, A276085(k)) == 1 */
static bool good(uint64_t k)
{
    if (k == 1) return true;
    ensure_primes(k);
    uint64_t m = k, P = 1 % k, r = 0;
    for (size_t i = 0; i < g_npr && m > 1; ++i) {
        uint64_t p = g_pr[i];
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            r = (r + (e % k) * P) % k;
        }
        P = (P * (p % k)) % k;
    }
    uint64_t x = k, y = r;
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x == 1;
}

seq_t a(idx_t n)
{
    size_t want = (size_t)n;
    while (g_nterms < want) {
        if (good(g_nextk)) {
            if (g_nterms == g_cap) {
                g_cap = g_cap ? g_cap * 2 : 1024;
                g_terms = (uint64_t *)realloc(g_terms, g_cap * sizeof(uint64_t));
            }
            g_terms[g_nterms++] = g_nextk;
        }
        ++g_nextk;
    }
    return (seq_t)g_terms[want - 1];
}
