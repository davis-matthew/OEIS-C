/*
 * A325541  Difference sequence of A325539.
 * https://oeis.org/A325541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A325539(n+1) - A325539(n), where A325539 is built by a
 *            greedy sieve: A325539(n) is the least positive integer not equal
 *            to 2*A325539(m)+1 or 3*A325539(m)+2 for any m < n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9834 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static uint64_t *g_terms = NULL;
static size_t g_len = 0, g_cap = 0;
static unsigned char *g_used = NULL;
static size_t g_ucap = 0;
static uint64_t g_next = 1;

static void g_ensure_used(size_t need)
{
    if (need < g_ucap) return;
    size_t nc = g_ucap ? g_ucap * 2 : 1024;
    while (nc <= need) nc *= 2;
    unsigned char *t = (unsigned char *)realloc(g_used, nc);
    if (!t) abort();
    for (size_t i = g_ucap; i < nc; ++i) t[i] = 0;
    g_used = t;
    g_ucap = nc;
}

static void g_gen(size_t count)
{
    while (g_len < count) {
        g_ensure_used((size_t)g_next);
        while (g_used[(size_t)g_next]) {
            ++g_next;
            g_ensure_used((size_t)g_next);
        }
        uint64_t v = g_next;
        if (g_len >= g_cap) {
            size_t nc = g_cap ? g_cap * 2 : 64;
            uint64_t *t = (uint64_t *)realloc(g_terms, nc * sizeof(uint64_t));
            if (!t) abort();
            g_terms = t;
            g_cap = nc;
        }
        g_terms[g_len++] = v;
        g_used[(size_t)v] = 1;
        g_ensure_used((size_t)(2 * v + 1));
        g_used[(size_t)(2 * v + 1)] = 1;
        g_ensure_used((size_t)(3 * v + 2));
        g_used[(size_t)(3 * v + 2)] = 1;
        ++g_next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    g_gen((size_t)n + 1);
    return (seq_t)(g_terms[n] - g_terms[n - 1]);
}
