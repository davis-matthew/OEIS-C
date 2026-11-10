/*
 * A305732  Heinz numbers of reducible integer partitions. Numbers n > 1 that
 *          are prime or whose prime indices are relatively prime and such
 *          that A181819(n) is already in the sequence.
 * https://oeis.org/A305732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer m>1 that is prime, or has gcd of prime
 *            indices = 1 and A181819(m) already in the sequence (search with
 *            recursive membership test)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

static uint64_t g_lim = 0;
static unsigned char *g_comp = NULL;
static uint32_t *g_pi = NULL;
static uint32_t *g_pr = NULL;
static size_t g_npr = 0;

static void A305732_ensure(uint64_t L)
{
    if (L <= g_lim) return;
    uint64_t nl = 2 * g_lim;
    if (nl < L) nl = L;
    if (nl < 1000) nl = 1000;
    free(g_comp); free(g_pi); free(g_pr);
    g_comp = (unsigned char *)calloc((size_t)nl + 1, 1);
    g_pi = (uint32_t *)malloc(((size_t)nl + 1) * sizeof(uint32_t));
    g_pr = (uint32_t *)malloc(((size_t)nl / 2 + 16) * sizeof(uint32_t));
    if (!g_comp || !g_pi || !g_pr) abort();
    g_comp[0] = g_comp[1] = 1;
    for (uint64_t i = 2; i * i <= nl; i++)
        if (!g_comp[i])
            for (uint64_t j = i * i; j <= nl; j += i) g_comp[j] = 1;
    g_npr = 0;
    uint32_t c = 0;
    for (uint64_t i = 0; i <= nl; i++) {
        if (i >= 2 && !g_comp[i]) { g_pr[g_npr++] = (uint32_t)i; c++; }
        g_pi[i] = c;
    }
    g_lim = nl;
}

static uint64_t A305732_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static bool A305732_member(uint64_t m)
{
    if (m < 2) return false;
    A305732_ensure(m);
    if (!g_comp[m]) return true;
    uint64_t g = 0, shadow = 1;
    uint64_t x = m;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) { x /= p; e++; }
            g = A305732_gcd(g, g_pi[p]);
            A305732_ensure(e + 1000);
            shadow *= g_pr[e - 1];
        }
    }
    if (x > 1) {
        g = A305732_gcd(g, g_pi[x]);
        shadow *= 2; /* prime(1) */
    }
    if (g != 1) return false;
    return A305732_member(shadow);
}

static uint64_t *g_terms = NULL;
static size_t g_cnt = 0, g_cap = 0;
static uint64_t g_next = 2;

seq_t a(idx_t n)
{
    while (g_cnt < (size_t)n) {
        if (A305732_member(g_next)) {
            if (g_cnt == g_cap) {
                g_cap = g_cap ? g_cap * 2 : 256;
                g_terms = (uint64_t *)realloc(g_terms, g_cap * sizeof(uint64_t));
                if (!g_terms) abort();
            }
            g_terms[g_cnt++] = g_next;
        }
        g_next++;
    }
    return (seq_t)g_terms[(size_t)n - 1];
}
