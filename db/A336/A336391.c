/*
 * A336391  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A331410(i) = A331410(j) and A336158(i) = A336158(j), for all i,
 *          j >= 1.
 * https://oeis.org/A336391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [A331410(n), A336158(n)]; computed incrementally by assigning
 *            ids to pairs in order of first appearance (hash table, memoized)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9776 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t s_lpf(uint64_t x)
{
    uint64_t best = 1;
    while ((x & 1) == 0) { best = 2; x >>= 1; }
    for (uint64_t d = 3; d * d <= x; d += 2) {
        while (x % d == 0) { best = d; x /= d; }
    }
    if (x > 1) best = x;
    return best;
}

static uint64_t s_A331410(uint64_t m)
{
    uint64_t k = m, cnt = 0;
    while (k & (k - 1)) {
        uint64_t p = s_lpf(k);
        k += k / p;
        ++cnt;
    }
    return cnt;
}

static uint64_t s_A336158(uint64_t m)
{
    static const uint64_t sp[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    while ((m & 1) == 0) m >>= 1;
    if (m == 1) return 1;
    unsigned ex[16];
    int c = 0;
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            unsigned e = 0;
            while (m % d == 0) { m /= d; ++e; }
            ex[c++] = e;
        }
    }
    if (m > 1) ex[c++] = 1;
    for (int i = 0; i < c; ++i)
        for (int j = i + 1; j < c; ++j)
            if (ex[i] < ex[j]) { unsigned t = ex[i]; ex[i] = ex[j]; ex[j] = t; }
    uint64_t r = 1;
    for (int i = 0; i < c; ++i)
        for (unsigned e = 0; e < ex[i]; ++e) r *= sp[i];
    return r;
}

static uint32_t *g_ids = NULL;
static size_t g_done = 0, g_idcap = 0;
static uint64_t *g_ka = NULL, *g_kb = NULL;
static size_t g_nd = 0, g_kcap = 0;
static int32_t *g_tab = NULL;
static size_t g_tcap = 0;

static size_t s_hash(uint64_t a, uint64_t b, size_t cap)
{
    uint64_t h = a * 0x9E3779B97F4A7C15ULL ^ (b + 0x7F4A7C15ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    h *= 0xBF58476D1CE4E5B9ULL;
    h ^= h >> 32;
    return (size_t)(h & (cap - 1));
}

static void s_rebuild(size_t ncap)
{
    free(g_tab);
    g_tab = (int32_t *)calloc(ncap, sizeof(int32_t));
    g_tcap = ncap;
    for (size_t i = 0; i < g_nd; ++i) {
        size_t h = s_hash(g_ka[i], g_kb[i], ncap);
        while (g_tab[h]) h = (h + 1) & (ncap - 1);
        g_tab[h] = (int32_t)(i + 1);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t N = (size_t)n;
    if (N > g_idcap) {
        size_t nc = g_idcap ? g_idcap : 1024;
        while (nc < N) nc *= 2;
        g_ids = (uint32_t *)realloc(g_ids, (nc + 1) * sizeof(uint32_t));
        g_idcap = nc;
    }
    if (!g_tab) s_rebuild(1024);
    while (g_done < N) {
        size_t m = ++g_done;
        uint64_t ka = s_A331410(m), kb = s_A336158(m);
        size_t h = s_hash(ka, kb, g_tcap);
        uint32_t id = 0;
        while (g_tab[h]) {
            size_t i = (size_t)g_tab[h] - 1;
            if (g_ka[i] == ka && g_kb[i] == kb) { id = (uint32_t)(i + 1); break; }
            h = (h + 1) & (g_tcap - 1);
        }
        if (!id) {
            if (g_nd >= g_kcap) {
                size_t nc = g_kcap ? g_kcap * 2 : 1024;
                g_ka = (uint64_t *)realloc(g_ka, nc * sizeof(uint64_t));
                g_kb = (uint64_t *)realloc(g_kb, nc * sizeof(uint64_t));
                g_kcap = nc;
            }
            g_ka[g_nd] = ka;
            g_kb[g_nd] = kb;
            ++g_nd;
            id = (uint32_t)g_nd;
            g_tab[h] = (int32_t)id;
            if (g_nd * 2 > g_tcap) s_rebuild(g_tcap * 2);
        }
        g_ids[m] = id;
    }
    return (seq_t)g_ids[N];
}
