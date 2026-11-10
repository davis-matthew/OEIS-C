/*
 * A371237  Inverse permutation to A371236.
 * https://oeis.org/A371237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A371236 (greedy rows, row r all
 *            congruent mod r); computed by simulating the triangle until n
 *            appears
 * Range:     u8 n <= 57, u16 n <= 2537, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9797 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned char *iu_used = NULL;
static size_t iu_used_cap = 0;
static uint64_t *iu_pos = NULL;
static uint64_t iu_cap = 0;
static uint64_t iu_rows = 0;
static uint64_t iu_count = 0;
static uint64_t iu_low = 1;

static void iu_need(uint64_t need)
{
    if (need < iu_used_cap) return;
    size_t nc = iu_used_cap ? iu_used_cap : 1024;
    while (nc <= need) nc <<= 1;
    unsigned char *t = (unsigned char *)realloc(iu_used, nc);
    if (!t) abort();
    memset(t + iu_used_cap, 0, nc - iu_used_cap);
    iu_used = t;
    iu_used_cap = nc;
}

static void iu_place(uint64_t v)
{
    iu_need(v);
    iu_used[v] = 1;
    iu_count++;
    if (v <= iu_cap) iu_pos[v] = iu_count;
}

static void iu_reset(uint64_t cap)
{
    free(iu_pos);
    iu_pos = (uint64_t *)calloc((size_t)cap + 1, sizeof(uint64_t));
    if (!iu_pos) abort();
    iu_cap = cap;
    if (iu_used) memset(iu_used, 0, iu_used_cap);
    iu_rows = 0;
    iu_count = 0;
    iu_low = 1;
}

static void iu_row(void)
{
    iu_rows++;
    uint64_t row = iu_rows;
    for (;;) {
        iu_need(iu_low);
        if (!iu_used[iu_low]) break;
        iu_low++;
    }
    uint64_t start = iu_low;
    iu_place(start);
    for (uint64_t j = 1; j < row; ++j) {
        uint64_t c = start + j * row;
        for (;;) {
            iu_need(c);
            if (!iu_used[c]) break;
            c += row;
        }
        iu_place(c);
    }
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    if (m == 0) return (seq_t)0;
    if (m > iu_cap || iu_pos == NULL) {
        uint64_t cap = 2 * m;
        if (cap < 1024) cap = 1024;
        iu_reset(cap);
    }
    while (iu_pos[m] == 0)
        iu_row();
    return (seq_t)iu_pos[m];
}
