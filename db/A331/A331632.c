/*
 * A331632  Distinct values of A055945 in order of their appearance as n
 *          grows.
 * https://oeis.org/A331632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = n-th distinct value in order of first appearance of
 *            A055945(m) = m - reverse_bits(m), m = 0,1,2,...; computed by
 *            scanning m with a hash set of seen values
 * Range:     i8 n <= 56, i16 n <= 4138, i32 n <= 8032, i64 n <= 8032,
 *            i128 n <= 8032, big any n
 * Validated: standard, pass, 79 terms through n = 7854 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static long long *g_list = 0;     /* distinct values in order */
static size_t g_cnt = 0, g_cap = 0;
static long long *g_tab = 0;      /* hash table of values */
static unsigned char *g_used = 0;
static size_t g_tcap = 0;
static uint64_t g_next = 0;       /* next m to scan */

static size_t hv(long long v, size_t mask) {
    uint64_t x = (uint64_t)v * 0x9E3779B97F4A7C15ULL;
    x ^= x >> 29;
    return (size_t)x & mask;
}

static int ht_insert(long long v) { /* returns 1 if new */
    size_t mask = g_tcap - 1;
    size_t i = hv(v, mask);
    while (g_used[i]) {
        if (g_tab[i] == v) return 0;
        i = (i + 1) & mask;
    }
    g_used[i] = 1;
    g_tab[i] = v;
    return 1;
}

static void ht_grow(void) {
    size_t oc = g_tcap;
    long long *ot = g_tab;
    unsigned char *ou = g_used;
    g_tcap = oc ? oc * 2 : 1024;
    g_tab = (long long *)malloc(g_tcap * sizeof(long long));
    g_used = (unsigned char *)calloc(g_tcap, 1);
    for (size_t i = 0; i < oc; i++)
        if (ou[i]) ht_insert(ot[i]);
    free(ot);
    free(ou);
}

static long long A055945_val(uint64_t m) {
    if (m == 0) return 0;
    uint64_t rev = 0, x = m;
    while (x) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return (long long)m - (long long)rev;
}

seq_t a(idx_t n) {
    while (g_cnt < (size_t)n) {
        if (g_tcap == 0 || (g_cnt + 1) * 2 > g_tcap) ht_grow();
        long long v = A055945_val(g_next++);
        if (ht_insert(v)) {
            if (g_cnt == g_cap) {
                g_cap = g_cap ? g_cap * 2 : 1024;
                g_list = (long long *)realloc(g_list, g_cap * sizeof(long long));
            }
            g_list[g_cnt++] = v;
        }
    }
    return (seq_t)g_list[n - 1];
}
