/*
 * A387523  Primes in the order in which they first divide a term of A386482.
 * https://oeis.org/A387523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime in order of first dividing a term of A386482
 *            (A386482: a(1)=1, a(2)=2, then greatest unused k<a(n-1) with
 *            gcd(k,a(n-1))>1, else smallest unused k>a(n-1) with gcd>1);
 *            computed by simulation
 * Range:     u8 n <= 54, u16 n <= 841, u32 n <= 841, u64 n <= 841,
 *            u128 n <= 841, big any n
 * Validated: standard, pass, 88 terms through n = 743 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint8_t *g_used = NULL;
static uint8_t *g_seen = NULL;
static size_t g_cap = 0;
static uint64_t *g_primes = NULL;
static size_t g_pcnt = 0, g_pcap = 0;
static uint64_t g_prev = 0;

static void a387523_ensure(size_t need)
{
    if (need < g_cap) return;
    size_t nc = g_cap ? g_cap : 1024;
    while (nc <= need) nc <<= 1;
    uint8_t *u = (uint8_t *)realloc(g_used, nc);
    uint8_t *s = (uint8_t *)realloc(g_seen, nc);
    if (!u || !s) abort();
    memset(u + g_cap, 0, nc - g_cap);
    memset(s + g_cap, 0, nc - g_cap);
    g_used = u;
    g_seen = s;
    g_cap = nc;
}

static size_t a387523_factor(uint64_t x, uint64_t *ps)
{
    size_t c = 0;
    for (uint64_t d = 2; d * d <= x; d++) {
        if (x % d == 0) {
            ps[c++] = d;
            while (x % d == 0) x /= d;
        }
    }
    if (x > 1) ps[c++] = x;
    return c;
}

static void a387523_note(uint64_t t)
{
    uint64_t ps[24];
    size_t c = a387523_factor(t, ps);
    for (size_t i = 0; i < c; i++) {
        a387523_ensure((size_t)ps[i]);
        if (!g_seen[ps[i]]) {
            g_seen[ps[i]] = 1;
            if (g_pcnt == g_pcap) {
                g_pcap = g_pcap ? g_pcap * 2 : 64;
                g_primes = (uint64_t *)realloc(g_primes, g_pcap * sizeof(uint64_t));
                if (!g_primes) abort();
            }
            g_primes[g_pcnt++] = ps[i];
        }
    }
}

static void a387523_step(void)
{
    uint64_t prev = g_prev;
    uint64_t ps[24];
    size_t c = a387523_factor(prev, ps);
    uint64_t best = 0;
    for (size_t i = 0; i < c; i++) {
        uint64_t p = ps[i];
        uint64_t m = ((prev - 1) / p) * p;
        while (m >= p && g_used[m]) m -= p;
        if (m >= p && m > best) best = m;
    }
    if (!best) {
        for (size_t i = 0; i < c; i++) {
            uint64_t p = ps[i];
            uint64_t m = (prev / p + 1) * p;
            a387523_ensure((size_t)m);
            while (g_used[m]) {
                m += p;
                a387523_ensure((size_t)m);
            }
            if (!best || m < best) best = m;
        }
    }
    a387523_ensure((size_t)best);
    g_used[best] = 1;
    g_prev = best;
    a387523_note(best);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!g_used) {
        a387523_ensure(1024);
        g_used[1] = 1;
        g_used[2] = 1;
        g_prev = 2;
        a387523_note(2);
    }
    while (g_pcnt < (size_t)n) a387523_step();
    return (seq_t)g_primes[(size_t)n - 1];
}
