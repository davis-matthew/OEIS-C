/*
 * A348249  a(n) = A348409(n) - A348246(n) + 1.
 * https://oeis.org/A348249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A348409(n) - A348246(n) + 1, i.e. the number of pairs
 *            from the first to the n-th (last) pair of consecutive terms of
 *            A307720 whose product is n. Computed by simulating the greedy
 *            construction of A307720. Runs of alternating terms u,w,u,w,...
 *            (all with product u*w) are added in one step
 * Range:     u8 n <= 44, u16 n <= 516, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9322 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *g_cnt = NULL;   /* pairs so far with product p            */
static size_t g_ccap = 0;
static uint64_t *g_first = NULL; /* index of first pair with product p<=LIM */
static uint64_t *g_last = NULL;  /* index of p-th pair with product p<=LIM  */
static size_t g_lim = 0;
static uint64_t *g_ptr = NULL;   /* per value v: smallest y with v*y not full */
static size_t g_pcap = 0;
static uint64_t g_pairs = 0;
static uint64_t g_prev = 1;
static int g_init = 0;

static void g_grow_c(size_t need)
{
    size_t nc = g_ccap ? g_ccap : 1024;
    while (nc <= need) nc <<= 1;
    if (nc == g_ccap) return;
    g_cnt = (uint32_t *)realloc(g_cnt, nc * sizeof(uint32_t));
    memset(g_cnt + g_ccap, 0, (nc - g_ccap) * sizeof(uint32_t));
    g_ccap = nc;
}

static void g_grow_p(size_t need)
{
    size_t nc = g_pcap ? g_pcap : 1024;
    size_t i;
    while (nc <= need) nc <<= 1;
    if (nc == g_pcap) return;
    g_ptr = (uint64_t *)realloc(g_ptr, nc * sizeof(uint64_t));
    for (i = g_pcap; i < nc; ++i) g_ptr[i] = 1;
    g_pcap = nc;
}

static void g_reset(size_t lim)
{
    free(g_cnt); free(g_first); free(g_last); free(g_ptr);
    g_cnt = NULL; g_ccap = 0;
    g_ptr = NULL; g_pcap = 0;
    g_lim = lim;
    g_first = (uint64_t *)calloc(lim + 1, sizeof(uint64_t));
    g_last = (uint64_t *)calloc(lim + 1, sizeof(uint64_t));
    g_grow_c(4 * lim + 16);
    g_grow_p(lim + 16);
    g_pairs = 0;
    g_prev = 1;   /* A307720(1) = 1 */
    g_init = 1;
}

static uint64_t g_findptr(uint64_t v)
{
    uint64_t x, p;
    if (v >= g_pcap) g_grow_p((size_t)v);
    x = g_ptr[v];
    for (;;) {
        p = v * x;
        if (p >= g_ccap) g_grow_c((size_t)p);
        if ((uint64_t)g_cnt[p] < p) break;
        x++;
    }
    g_ptr[v] = x;
    return x;
}

static void g_rec(uint64_t p, uint64_t k)
{
    if (g_cnt[p] == 0 && p <= g_lim) g_first[p] = g_pairs + 1;
    g_cnt[p] += (uint32_t)k;
    g_pairs += k;
    if ((uint64_t)g_cnt[p] == p && p <= g_lim) g_last[p] = g_pairs;
}

seq_t a(idx_t n)
{
    size_t nn = (size_t)n;
    if (n == 0) return (seq_t)0;
    if (!g_init || nn > g_lim) {
        size_t lim = 2 * nn;
        if (lim < 65536) lim = 65536;
        g_reset(lim);
    }
    while ((uint64_t)g_cnt[nn] < (uint64_t)nn) {
        uint64_t v = g_prev;
        uint64_t x = g_findptr(v);
        uint64_t p = v * x;
        g_rec(p, 1);
        g_prev = x;
        if ((uint64_t)g_cnt[p] < p) {
            uint64_t y = g_findptr(x);
            if (y == v) {
                uint64_t r = p - (uint64_t)g_cnt[p];
                g_rec(p, r);
                if (r & 1) g_prev = v;
            }
        }
    }
    return (seq_t)(g_last[nn] - g_first[nn] + 1);
}
