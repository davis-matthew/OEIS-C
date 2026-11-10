/*
 * A324389  Lexicographically earliest sequence such that a(i) = a(j) => f(i)
 *          = f(j), where f(n) = [A009194(n), A318458(n)] for all other
 *          numbers, except f(1) = -1.
 * https://oeis.org/A324389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = restricted-growth labelling of f(n) = [A009194(n),
 *            A318458(n)] with f(1) = -1, i.e. a(n) = 1 + (number of distinct
 *            f-values among f(1..n-1)) if f(n) is new, else a(m) for the
 *            first m < n with f(m) = f(n). Computed by scanning n = 1, 2, ...
 *            and assigning labels in order of first appearance
 * Range:     u8 n <= 567, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 119 terms through n = 8904 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *g_gk = 0, *g_bk = 0;   /* distinct keys, in order of first appearance */
static uint32_t *g_lab = 0;             /* label of each m (1-based) */
static size_t g_labcap = 0, g_done = 0, g_nd = 0, g_keycap = 0;
static int64_t *g_tab = 0;
static size_t g_tabcap = 0;

static uint64_t hk_hash(uint64_t g, uint64_t b)
{
    uint64_t h = g * 0x9E3779B97F4A7C15ULL ^ (b + 0x7F4A7C15ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    h *= 0xBF58476D1CE4E5B9ULL;
    h ^= h >> 32;
    return h;
}

static void hk_rebuild(size_t newcap)
{
    free(g_tab);
    g_tab = (int64_t *)malloc(newcap * sizeof(int64_t));
    g_tabcap = newcap;
    for (size_t i = 0; i < newcap; ++i) g_tab[i] = -1;
    for (size_t k = 0; k < g_nd; ++k) {
        size_t p = (size_t)(hk_hash(g_gk[k], g_bk[k]) & (newcap - 1));
        while (g_tab[p] >= 0) p = (p + 1) & (newcap - 1);
        g_tab[p] = (int64_t)k;
    }
}

static uint64_t gcd_a324389(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t sigma_a324389(uint64_t n)
{
    uint64_t s = 1, m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t term = 1, pw = 1;
            while (m % p == 0) { m /= p; pw *= p; term += pw; }
            s *= term;
        }
    }
    if (m > 1) s *= (m + 1);
    return s;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (g_tab == 0) {
        g_labcap = 1024;
        g_lab = (uint32_t *)malloc(g_labcap * sizeof(uint32_t));
        g_keycap = 1024;
        g_gk = (uint64_t *)malloc(g_keycap * sizeof(uint64_t));
        g_bk = (uint64_t *)malloc(g_keycap * sizeof(uint64_t));
        hk_rebuild(4096);
    }
    if ((size_t)n >= g_labcap) {
        size_t nc = g_labcap;
        while (nc <= (size_t)n) nc *= 2;
        g_lab = (uint32_t *)realloc(g_lab, nc * sizeof(uint32_t));
        g_labcap = nc;
    }
    while (g_done < (size_t)n) {
        uint64_t m = (uint64_t)g_done + 1;
        uint64_t g, b;
        if (m == 1) { g = 0; b = 0; }          /* unique sentinel: gcd >= 1 for m > 1 */
        else {
            uint64_t s = sigma_a324389(m);
            g = gcd_a324389(m, s);
            b = m & (s - m);
        }
        size_t p = (size_t)(hk_hash(g, b) & (g_tabcap - 1));
        int64_t found = -1;
        while (g_tab[p] >= 0) {
            size_t k = (size_t)g_tab[p];
            if (g_gk[k] == g && g_bk[k] == b) { found = (int64_t)k; break; }
            p = (p + 1) & (g_tabcap - 1);
        }
        if (found < 0) {
            if (g_nd >= g_keycap) {
                g_keycap *= 2;
                g_gk = (uint64_t *)realloc(g_gk, g_keycap * sizeof(uint64_t));
                g_bk = (uint64_t *)realloc(g_bk, g_keycap * sizeof(uint64_t));
            }
            g_gk[g_nd] = g;
            g_bk[g_nd] = b;
            g_tab[p] = (int64_t)g_nd;
            found = (int64_t)g_nd;
            g_nd++;
            if (g_nd * 2 > g_tabcap) hk_rebuild(g_tabcap * 2);
        }
        g_lab[m] = (uint32_t)(found + 1);
        g_done++;
    }
    return (seq_t)g_lab[(size_t)n];
}
