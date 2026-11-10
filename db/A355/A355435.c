/*
 * A355435  Lexicographically earliest sequence of distinct positive integers
 *          such that for any n > 1, a(n) is a multiple of a(A080079(n-1)).
 * https://oeis.org/A355435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(1)=1; for n>1, a(n) = smallest unused positive multiple of
 *            a(A080079(n-1)), where A080079(m) = 2p - m with p the greatest
 *            power of 2 <= m (greedy search with a used-values hash set)
 * Range:     u8 n <= 73, u16 n <= 2157, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 80 terms through n = 7540 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static idx_t A080079_a(idx_t n)
{
    if (n == 0) return 0;
    idx_t p = 1;
    while (p <= (n >> 1)) p <<= 1;
    return p - (n - p);
}

static seq_t *g_vals = 0;
static unsigned long long *g_mult = 0;
static idx_t g_cnt = 0, g_cap = 0;

static unsigned long long *g_tab = 0;
static size_t g_tcap = 0, g_tcnt = 0;

static size_t g_hash(unsigned long long x, size_t cap)
{
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    return (size_t)(x & (cap - 1));
}

static bool g_used(unsigned long long key)
{
    if (!g_tcap) return false;
    size_t h = g_hash(key, g_tcap);
    while (g_tab[h]) {
        if (g_tab[h] == key) return true;
        h = (h + 1) & (g_tcap - 1);
    }
    return false;
}

static void g_insert_raw(unsigned long long key)
{
    size_t h = g_hash(key, g_tcap);
    while (g_tab[h]) h = (h + 1) & (g_tcap - 1);
    g_tab[h] = key;
}

static void g_insert(unsigned long long key)
{
    if ((g_tcnt + 1) * 2 > g_tcap) {
        size_t ncap = g_tcap ? g_tcap * 2 : 1024;
        unsigned long long *old = g_tab;
        size_t ocap = g_tcap;
        g_tab = (unsigned long long *)calloc(ncap, sizeof(unsigned long long));
        g_tcap = ncap;
        for (size_t i = 0; i < ocap; i++)
            if (old[i]) g_insert_raw(old[i]);
        free(old);
    }
    g_insert_raw(key);
    g_tcnt++;
}

static void g_grow(void)
{
    idx_t ncap = g_cap ? g_cap * 2 : 1024;
    seq_t *nv = mem_new(seq_t, (size_t)ncap + 1);
    unsigned long long *nm = (unsigned long long *)calloc((size_t)ncap + 1, sizeof(unsigned long long));
    for (idx_t i = 1; i <= g_cnt; i++) {
        nv[i] = g_vals[i];
        nm[i] = g_mult[i];
    }
    if (g_vals) mem_free(g_vals);
    free(g_mult);
    g_vals = nv;
    g_mult = nm;
    g_cap = ncap;
}

seq_t a(idx_t n)
{
    while (g_cnt < n) {
        idx_t i = g_cnt + 1;
        if (i > g_cap) g_grow();
        seq_t val;
        if (i == 1) {
            val = (seq_t)1;
        } else {
            idx_t j = A080079_a(i - 1);
            seq_t k = g_vals[j];
            unsigned long long m = g_mult[j];
            if (m == 0) m = 1;
            while (g_used((unsigned long long)(k * (seq_t)m))) m++;
            g_mult[j] = m;
            val = k * (seq_t)m;
        }
        g_vals[i] = val;
        g_insert((unsigned long long)val);
        g_cnt = i;
    }
    return g_vals[n];
}
