/*
 * A361251  Inverse permutation to A360371.
 * https://oeis.org/A361251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A360371(k) = n; computed by generating the
 *            triangle A360371 until n appears (search)
 * Range:     u8 n <= 108, u16 n <= 3016, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9897 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef unsigned long long a361251_u;

static a361251_u *a361251_pos = NULL;
static size_t a361251_pos_cap = 0;
static a361251_u *a361251_cbase = NULL;
static a361251_u *a361251_cmult = NULL;
static size_t a361251_ccap = 0;
static a361251_u a361251_next = 1;
static a361251_u a361251_idx = 0;
static a361251_u a361251_row = 1;
static a361251_u a361251_col = 0;

static void a361251_grow_pos(size_t need)
{
    if (need < a361251_pos_cap) return;
    size_t nc = a361251_pos_cap ? a361251_pos_cap : 1024;
    while (nc <= need) nc <<= 1;
    a361251_u *p = (a361251_u *)realloc(a361251_pos, nc * sizeof(a361251_u));
    if (!p) abort();
    for (size_t i = a361251_pos_cap; i < nc; ++i) p[i] = 0;
    a361251_pos = p;
    a361251_pos_cap = nc;
}

static void a361251_grow_col(size_t need)
{
    if (need < a361251_ccap) return;
    size_t nc = a361251_ccap ? a361251_ccap : 256;
    while (nc <= need) nc <<= 1;
    a361251_u *b = (a361251_u *)realloc(a361251_cbase, nc * sizeof(a361251_u));
    if (!b) abort();
    a361251_cbase = b;
    a361251_u *m = (a361251_u *)realloc(a361251_cmult, nc * sizeof(a361251_u));
    if (!m) abort();
    a361251_cmult = m;
    for (size_t i = a361251_ccap; i < nc; ++i) { b[i] = 0; m[i] = 0; }
    a361251_ccap = nc;
}

static void a361251_step(void)
{
    ++a361251_idx;
    ++a361251_col;
    if (a361251_col > a361251_row) { ++a361251_row; a361251_col = 1; }
    a361251_grow_col((size_t)a361251_col);
    a361251_u c = a361251_col;
    if (a361251_cbase[c] == 0) {
        for (;;) {
            a361251_grow_pos((size_t)a361251_next);
            if (a361251_pos[a361251_next] == 0) break;
            ++a361251_next;
        }
        a361251_u v = a361251_next++;
        a361251_pos[v] = a361251_idx;
        a361251_cbase[c] = v;
        a361251_cmult[c] = 2;
    } else {
        a361251_u base = a361251_cbase[c];
        a361251_u mult = a361251_cmult[c];
        for (;;) {
            a361251_u v = base * mult;
            a361251_grow_pos((size_t)v);
            if (a361251_pos[v] == 0) {
                a361251_pos[v] = a361251_idx;
                a361251_cmult[c] = mult + 1;
                break;
            }
            ++mult;
        }
    }
}

seq_t a(idx_t n)
{
    a361251_u m = (a361251_u)n;
    for (;;) {
        if (m < a361251_pos_cap && a361251_pos[m] != 0)
            return (seq_t)a361251_pos[m];
        a361251_step();
    }
}
