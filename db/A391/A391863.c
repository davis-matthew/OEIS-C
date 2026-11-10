/*
 * A391863  Numbers k that are coprime to A276085(k) and that are not
 *          divisible by p^p for any prime p, where A276085 is the primorial
 *          base log-function.
 * https://oeis.org/A391863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that gcd(k, A276085(k)) = 1 and no p^p
 *            divides k (search over k; A276085(k) tested mod each prime q |
 *            k)
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9999 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *g_pr = NULL;
static size_t g_prn = 0;
static uint64_t g_lim = 0;

static void g_ensure(uint64_t limit)
{
    if (limit <= g_lim) return;
    uint64_t nl = g_lim ? g_lim : 1024;
    while (nl < limit) nl <<= 1;
    unsigned char *s = (unsigned char *)calloc((size_t)nl + 1, 1);
    if (!s) abort();
    free(g_pr);
    g_pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)nl / 2 + 2));
    if (!g_pr) abort();
    g_prn = 0;
    for (uint64_t i = 2; i <= nl; ++i) {
        if (!s[i]) {
            g_pr[g_prn++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= nl; j += i) s[j] = 1;
        }
    }
    free(s);
    g_lim = nl;
}

static bool good_k(uint64_t k)
{
    if (k == 1) return true;
    uint64_t ps[20], es[20];
    int c = 0;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            if (e >= p) return false;
            ps[c] = p; es[c] = e; ++c;
        }
    }
    if (m > 1) { ps[c] = m; es[c] = 1; ++c; }
    g_ensure(k + 2);
    for (int qi = 0; qi < c; ++qi) {
        uint64_t q = ps[qi];
        uint64_t sum = 0;
        for (int pi = 0; pi < c; ++pi) {
            uint64_t p = ps[pi];
            if (p > q) break;
            uint64_t prod = 1 % q;
            for (size_t i = 0; i < g_prn && g_pr[i] < p; ++i)
                prod = (prod * g_pr[i]) % q;
            sum = (sum + (es[pi] % q) * prod) % q;
        }
        if (sum == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t *tab = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t next_k = 1;
    while (len < (size_t)n) {
        uint64_t k = next_k++;
        if (good_k(k)) {
            if (len == cap) {
                cap = cap ? cap * 2 : 1024;
                tab = (uint64_t *)realloc(tab, cap * sizeof(uint64_t));
                if (!tab) abort();
            }
            tab[len++] = k;
        }
    }
    return (seq_t)tab[(size_t)n - 1];
}
