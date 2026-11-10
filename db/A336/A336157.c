/*
 * A336157  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A318458(i) = A318458(j) and A336158(i) = A336158(j), for all i,
 *          j >= 1.
 * https://oeis.org/A336157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of the pairs [A318458(n),
 *            A336158(n)]; computed by scanning m=1..n and numbering distinct
 *            pairs in order of first appearance. A336158 is identified by the
 *            sorted exponent signature of the odd part of m
 * Range:     u8 n <= 617, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9766 (2026-11-08)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

#define SIGLEN 16

static uint32_t *g_ids = NULL;
static size_t g_ids_cap = 0;
static size_t g_done = 0;

static seq_t *g_xs = NULL;
static unsigned char *g_sigs = NULL;
static size_t g_cnt = 0, g_cap = 0;

static void A336157_pair(uint64_t m, seq_t *x, unsigned char *sig)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0, i, j, k = 0;
    factor_u64(m, pr, ex, &cnt);
    seq_t sg = 1;
    for (i = 0; i < SIGLEN; ++i) sig[i] = 0;
    for (i = 0; i < cnt; ++i) {
        seq_t term = 1, pw = 1;
        for (j = 0; j < ex[i]; ++j) {
            pw *= (seq_t)pr[i];
            term += pw;
        }
        sg *= term;
        if (pr[i] != 2 && k < SIGLEN) {
            sig[k++] = (unsigned char)ex[i];
        }
    }
    /* sort descending */
    for (i = 1; i < k; ++i) {
        unsigned char v = sig[i];
        size_t t = i;
        while (t > 0 && sig[t - 1] < v) { sig[t] = sig[t - 1]; --t; }
        sig[t] = v;
    }
    seq_t proper = sg - (seq_t)m;
    *x = (seq_t)m & proper;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    size_t N = (size_t)n;
    if (N + 1 > g_ids_cap) {
        size_t nc = N + 1;
        uint32_t *p = (uint32_t *)realloc(g_ids, nc * sizeof(uint32_t));
        if (!p) return 0;
        g_ids = p;
        g_ids_cap = nc;
    }
    while (g_done < N) {
        size_t m = g_done + 1;
        seq_t x;
        unsigned char sig[SIGLEN];
        A336157_pair((uint64_t)m, &x, sig);
        size_t found = (size_t)-1;
        for (size_t i = 0; i < g_cnt; ++i) {
            if (g_xs[i] == x && memcmp(g_sigs + i * SIGLEN, sig, SIGLEN) == 0) {
                found = i;
                break;
            }
        }
        if (found == (size_t)-1) {
            if (g_cnt == g_cap) {
                size_t nc = g_cap ? g_cap * 2 : 256;
                if (g_xs == NULL) g_xs = mem_new(seq_t, nc);
                else g_xs = mem_resize(seq_t, g_xs, nc);
                g_sigs = (unsigned char *)realloc(g_sigs, nc * SIGLEN);
                g_cap = nc;
            }
            g_xs[g_cnt] = x;
            memcpy(g_sigs + g_cnt * SIGLEN, sig, SIGLEN);
            found = g_cnt++;
        }
        g_ids[m] = (uint32_t)(found + 1);
        g_done = m;
    }
    return (seq_t)g_ids[N];
}
