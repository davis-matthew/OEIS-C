/*
 * A361348  Numbers k such that A361338(k) = 9.
 * https://oeis.org/A361348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that A361338(k) = 9; computed by an
 *            incremental sieve of reachable single-digit masks (every split
 *            product is < k)
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint16_t *g_mask = NULL;
static uint64_t g_cap = 0;
static uint64_t g_next = 0;      /* next k to process */
static uint64_t *g_terms = NULL;
static uint64_t g_tcnt = 0, g_tcap = 0;

static void A361348_step(void)
{
    uint64_t k = g_next;
    if (k >= g_cap) {
        uint64_t nc = g_cap ? g_cap * 2 : 1024;
        uint16_t *nm = (uint16_t *)realloc(g_mask, nc * sizeof(uint16_t));
        if (!nm) exit(1);
        g_mask = nm;
        g_cap = nc;
    }
    uint16_t m = 0;
    if (k <= 9) {
        m = (uint16_t)(1u << (unsigned)k);
    } else {
        uint64_t div = 10;
        while (div <= k) {
            uint64_t left = k / div, right = k % div;
            /* right has lenR digits where div = 10^lenR; disallow leading zeros */
            if (!(div > 10 && right < div / 10)) {
                m |= g_mask[left * right];
            }
            div *= 10;
        }
    }
    g_mask[k] = m;
    if (m) {
        int c = 0;
        uint16_t t = m;
        while (t) { c += t & 1; t >>= 1; }
        if (c == 9) {
            if (g_tcnt >= g_tcap) {
                uint64_t nc = g_tcap ? g_tcap * 2 : 256;
                uint64_t *nt = (uint64_t *)realloc(g_terms, nc * sizeof(uint64_t));
                if (!nt) exit(1);
                g_terms = nt;
                g_tcap = nc;
            }
            g_terms[g_tcnt++] = k;
        }
    }
    g_next++;
}

seq_t a(idx_t n)
{
    while (g_tcnt < (uint64_t)n)
        A361348_step();
    return (seq_t)g_terms[n - 1];
}
