/*
 * A336936  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), where f(n) = [A007733(n), A329697(n), A331410(n)],
 *          for all i, j >= 1.
 * https://oeis.org/A336936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of [A007733(n),
 *            A329697(n), A331410(n)]; computed by assigning class ids in
 *            order of first appearance (incremental, cached)
 * Range:     u8 n <= 586, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 8934 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t A336936_lpf(uint64_t m)
{
    uint64_t largest = 1;
    while ((m & 1ULL) == 0) { largest = 2; m >>= 1; }
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0) m /= f;
        }
    }
    if (m > 1) largest = m;
    return largest;
}

static bool A336936_pow2(uint64_t x) { return x && (x & (x - 1)) == 0; }

static uint64_t A336936_A007733(uint64_t n)
{
    while ((n & 1ULL) == 0) n >>= 1;
    if (n == 1) return 1;
    uint64_t p = 2 % n, ord = 1;
    while (p != 1) { p = (p * 2) % n; ord++; }
    return ord;
}

static uint64_t A336936_A329697(uint64_t n)
{
    uint64_t cnt = 0;
    while (!A336936_pow2(n)) {
        while ((n & 1ULL) == 0) n >>= 1;
        if (n == 1) break;
        uint64_t p = A336936_lpf(n);
        n = n - n / p;
        cnt++;
    }
    return cnt;
}

static uint64_t A336936_A331410(uint64_t n)
{
    uint64_t cnt = 0;
    while (!A336936_pow2(n)) {
        uint64_t p = A336936_lpf(n);
        n += n / p;
        cnt++;
    }
    return cnt;
}

static uint64_t *g_ids = 0;
static size_t g_n = 0, g_cap = 0;
static uint64_t *g_k1 = 0, *g_k2 = 0, *g_k3 = 0;
static size_t g_cls = 0, g_ccap = 0;
static size_t *g_tab = 0;
static size_t g_tsize = 0;

static size_t A336936_hash(uint64_t a, uint64_t b, uint64_t c)
{
    uint64_t h = a * 0x9E3779B97F4A7C15ULL;
    h ^= (h >> 29);
    h += b * 0xC2B2AE3D27D4EB4FULL;
    h ^= (h >> 31);
    h += c * 0x165667B19E3779F9ULL;
    h ^= (h >> 32);
    h *= 0x9E3779B97F4A7C15ULL;
    h ^= (h >> 29);
    return (size_t)h;
}

static void A336936_rebuild(size_t newsize)
{
    free(g_tab);
    g_tsize = newsize;
    g_tab = (size_t *)calloc(g_tsize, sizeof(size_t));
    for (size_t i = 0; i < g_cls; i++) {
        size_t h = A336936_hash(g_k1[i], g_k2[i], g_k3[i]) & (g_tsize - 1);
        while (g_tab[h]) h = (h + 1) & (g_tsize - 1);
        g_tab[h] = i + 1;
    }
}

seq_t a(idx_t n)
{
    if (g_tsize == 0) A336936_rebuild(1024);
    while (g_n < (size_t)n) {
        uint64_t m = (uint64_t)g_n + 1;
        uint64_t x = A336936_A007733(m);
        uint64_t y = A336936_A329697(m);
        uint64_t z = A336936_A331410(m);
        size_t h = A336936_hash(x, y, z) & (g_tsize - 1);
        size_t found = 0;
        while (g_tab[h]) {
            size_t i = g_tab[h] - 1;
            if (g_k1[i] == x && g_k2[i] == y && g_k3[i] == z) { found = i + 1; break; }
            h = (h + 1) & (g_tsize - 1);
        }
        if (!found) {
            if (g_cls == g_ccap) {
                g_ccap = g_ccap ? g_ccap * 2 : 1024;
                g_k1 = (uint64_t *)realloc(g_k1, g_ccap * sizeof(uint64_t));
                g_k2 = (uint64_t *)realloc(g_k2, g_ccap * sizeof(uint64_t));
                g_k3 = (uint64_t *)realloc(g_k3, g_ccap * sizeof(uint64_t));
            }
            g_k1[g_cls] = x; g_k2[g_cls] = y; g_k3[g_cls] = z;
            g_cls++;
            found = g_cls;
            g_tab[h] = found;
            if (g_cls * 2 > g_tsize) A336936_rebuild(g_tsize * 2);
        }
        if (g_n == g_cap) {
            g_cap = g_cap ? g_cap * 2 : 1024;
            g_ids = (uint64_t *)realloc(g_ids, g_cap * sizeof(uint64_t));
        }
        g_ids[g_n++] = (uint64_t)found;
    }
    return (seq_t)g_ids[(size_t)n - 1];
}
