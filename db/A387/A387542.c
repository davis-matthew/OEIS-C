/*
 * A387542  a(n) is the distance from the n-th term of A386482 to the nearest
 *          term of A386482 coprime to it.
 * https://oeis.org/A387542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least d >= 0 with gcd(b(n), b(n-d)) = 1 or gcd(b(n),
 *            b(n+d)) = 1, where b = A386482; computed by generating b with a
 *            greedy search, then scanning neighbours
 * Range:     u8 n <= 4258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9821 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint64_t *g_t = NULL;
static size_t g_len = 0, g_cap = 0;
static unsigned char *g_used = NULL;
static size_t g_ucap = 0;

static uint64_t g_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void g_need_used(size_t idx)
{
    if (idx < g_ucap) return;
    size_t nc = g_ucap ? g_ucap : 64;
    while (nc <= idx) nc <<= 1;
    unsigned char *p = (unsigned char *)realloc(g_used, nc);
    if (!p) abort();
    memset(p + g_ucap, 0, nc - g_ucap);
    g_used = p;
    g_ucap = nc;
}

static void g_push(uint64_t v)
{
    if (g_len == g_cap) {
        size_t nc = g_cap ? g_cap * 2 : 64;
        uint64_t *p = (uint64_t *)realloc(g_t, nc * sizeof(uint64_t));
        if (!p) abort();
        g_t = p;
        g_cap = nc;
    }
    g_t[g_len++] = v;
    g_need_used((size_t)v);
    g_used[(size_t)v] = 1;
}

static void g_generate(size_t target)
{
    if (g_len == 0) {
        g_push(1);
        g_push(2);
    }
    while (g_len < target) {
        uint64_t prev = g_t[g_len - 1];
        uint64_t cand = 0;
        bool found = false;
        for (uint64_t k = prev - 1; k >= 1; --k) {
            if (!g_used[(size_t)k] && g_gcd(prev, k) > 1) {
                cand = k;
                found = true;
                break;
            }
        }
        if (!found) {
            for (uint64_t k = prev + 1;; ++k) {
                g_need_used((size_t)k);
                if (!g_used[(size_t)k] && g_gcd(prev, k) > 1) {
                    cand = k;
                    break;
                }
            }
        }
        g_push(cand);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t N = (size_t)n;
    g_generate(N + 32);
    uint64_t v = g_t[N - 1];
    for (size_t d = 0;; ++d) {
        if (d < N) {
            if (g_gcd(v, g_t[N - 1 - d]) == 1) return (seq_t)d;
        }
        if (N + d > g_len) g_generate((N + d) * 2);
        if (g_gcd(v, g_t[N - 1 + d]) == 1) return (seq_t)d;
    }
}
