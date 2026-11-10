/*
 * A357396  Inverse of A357379.
 * https://oeis.org/A357396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m such that A357379(m) = n, where A357379(m) =
 *            A357378(floor(m/2))*A357378(m); computed by generating A357378
 *            incrementally (search) and recording the first index of each
 *            value
 * Range:     u8 n <= 46, u16 n <= 970, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 8564 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned long long *g_T = NULL;   /* A357378 values */
static unsigned long long *g_V = NULL;   /* A357379 values */
static size_t g_len = 0, g_cap = 0;
static unsigned char *g_used = NULL;
static size_t g_used_cap = 0;
static unsigned long long *g_pos = NULL; /* value -> index+1 */
static size_t g_pos_cap = 0;

static void A357396_use_grow(size_t need)
{
    if (need <= g_used_cap) return;
    size_t nc = g_used_cap ? g_used_cap : 1024;
    while (nc < need) nc *= 2;
    unsigned char *t = (unsigned char *)realloc(g_used, nc);
    if (!t) abort();
    memset(t + g_used_cap, 0, nc - g_used_cap);
    g_used = t;
    g_used_cap = nc;
}

static void A357396_init(void)
{
    g_cap = 1024;
    g_T = (unsigned long long *)malloc(g_cap * sizeof(unsigned long long));
    g_V = (unsigned long long *)malloc(g_cap * sizeof(unsigned long long));
    if (!g_T || !g_V) abort();
    A357396_use_grow(16);
    g_T[0] = 1;
    g_V[0] = 1;
    g_used[1] = 1;
    g_len = 1;
    g_pos_cap = 0;
}

static void A357396_pos_grow(size_t need)
{
    if (need <= g_pos_cap) return;
    size_t nc = g_pos_cap ? g_pos_cap : 1024;
    while (nc < need) nc *= 2;
    unsigned long long *t = (unsigned long long *)calloc(nc, sizeof(unsigned long long));
    if (!t) abort();
    free(g_pos);
    g_pos = t;
    g_pos_cap = nc;
    for (size_t i = 0; i < g_len; i++) {
        unsigned long long v = g_V[i];
        if (v < g_pos_cap && !g_pos[v]) g_pos[v] = (unsigned long long)i + 1;
    }
}

static void A357396_step(void)
{
    size_t i = g_len;
    if (i >= g_cap) {
        size_t nc = g_cap * 2;
        unsigned long long *a = (unsigned long long *)realloc(g_T, nc * sizeof(unsigned long long));
        if (!a) abort();
        g_T = a;
        unsigned long long *b = (unsigned long long *)realloc(g_V, nc * sizeof(unsigned long long));
        if (!b) abort();
        g_V = b;
        g_cap = nc;
    }
    unsigned long long tk = g_T[i >> 1];
    unsigned long long c = 1, prod;
    for (;;) {
        prod = c * tk;
        A357396_use_grow((size_t)prod + 1);
        if (!g_used[prod]) break;
        c++;
    }
    g_used[prod] = 1;
    g_T[i] = c;
    unsigned long long v = c * tk;
    g_V[i] = v;
    g_len = i + 1;
    if (v < g_pos_cap && !g_pos[v]) g_pos[v] = (unsigned long long)i + 1;
}

seq_t a(idx_t n)
{
    if (!g_T) A357396_init();
    A357396_pos_grow((size_t)n + 1);
    while (!g_pos[(size_t)n]) A357396_step();
    return (seq_t)(g_pos[(size_t)n] - 1);
}
