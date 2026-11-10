/*
 * A370635  a(n) is the index m of the smallest term k = A135507(m) such that
 *          prime(n) | k.
 * https://oeis.org/A370635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m with prime(n) | A135507(m), where A135507(1)=1,
 *            A135507(m)=A135507(m-1)*(2+m/gcd(A135507(m-1),m)); computed by
 *            tracking prime exponents of A135507 and recording the first m at
 *            which each prime appears
 * Range:     u8 n <= 9, u16 n <= 285, u32 n <= 2074, u64 n <= 2074,
 *            u128 n <= 2074, big any n
 * Validated: standard, pass, 70 terms through n = 2055 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint32_t *g_spf = NULL;
static unsigned *g_ex = NULL;
static uint64_t *g_first = NULL;
static size_t g_cap = 0;
static uint64_t g_cur = 1;

static void A370635_ensure(size_t need)
{
    if (need <= g_cap) return;
    size_t nc = g_cap ? g_cap : 1024;
    while (nc < need) nc *= 2;
    g_spf = (uint32_t *)realloc(g_spf, nc * sizeof(uint32_t));
    g_ex = (unsigned *)realloc(g_ex, nc * sizeof(unsigned));
    g_first = (uint64_t *)realloc(g_first, nc * sizeof(uint64_t));
    if (!g_spf || !g_ex || !g_first) exit(1);
    for (size_t i = g_cap; i < nc; ++i) { g_ex[i] = 0; g_first[i] = 0; }
    for (size_t i = 0; i < nc; ++i) g_spf[i] = 0;
    for (size_t i = 2; i < nc; ++i) {
        if (g_spf[i] == 0) {
            for (size_t j = i; j < nc; j += i)
                if (g_spf[j] == 0) g_spf[j] = (uint32_t)i;
        }
    }
    g_cap = nc;
}

static void A370635_step(void)
{
    uint64_t m = ++g_cur;
    A370635_ensure((size_t)m + 3);
    uint64_t g = 1, x = m;
    while (x > 1) {
        uint32_t p = g_spf[x];
        unsigned e = 0;
        while (x % p == 0) { x /= p; e++; }
        unsigned k = e < g_ex[p] ? e : g_ex[p];
        while (k--) g *= p;
    }
    uint64_t t = 2 + m / g;
    while (t > 1) {
        uint32_t p = g_spf[t];
        unsigned e = 0;
        while (t % p == 0) { t /= p; e++; }
        if (g_ex[p] == 0) g_first[p] = m;
        g_ex[p] += e;
    }
}

static uint64_t A370635_nth_prime(uint64_t n)
{
    size_t limit = 64;
    for (;;) {
        char *comp = (char *)calloc(limit + 1, 1);
        if (!comp) exit(1);
        uint64_t cnt = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (!comp[i]) {
                if (++cnt == n) { free(comp); return i; }
                for (size_t j = i * i; j <= limit; j += i) comp[j] = 1;
            }
        }
        free(comp);
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t p = A370635_nth_prime((uint64_t)n);
    A370635_ensure((size_t)p + 3);
    while (g_first[p] == 0) {
        A370635_step();
    }
    return (seq_t)g_first[p];
}
