/*
 * A375112  Triangle T(n, k), n > 0, k = 0..n-1, read by rows; T(n, k) is the
 *          unique m such that A375110(m+1) = n and A375110(m) mod
 *          A375110(m+1) = k.
 * https://oeis.org/A375112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = the unique m with A375110(m+1) = n and A375110(m) mod n
 *            = k; flat index N -> row n, column k. Computed by generating
 *            A375110 (greedy search with per-value residue bitsets) and
 *            recording first occurrences
 * Range:     u8 n <= 164, u16 n <= 10011, u32 n <= 10011, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 94 terms through n = 9432 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

static uint64_t *T375112_b = NULL;      /* b[1..len], 1-based */
static size_t T375112_len = 0, T375112_cap = 0;
static uint64_t **T375112_masks = NULL;
static size_t T375112_mcap = 0;
static uint64_t *T375112_tab = NULL;    /* tab[N], N 1-based flat index */
static size_t T375112_tcap = 0;

static void T375112_record(uint64_t m, uint64_t c, uint64_t r)
{
    uint64_t N = c * (c - 1) / 2 + r + 1;
    if (N < T375112_tcap && T375112_tab[N] == 0) T375112_tab[N] = m;
}

static void T375112_push(uint64_t v)
{
    if (T375112_len + 2 > T375112_cap) {
        size_t nc = T375112_cap ? T375112_cap * 2 : 1024;
        uint64_t *p = (uint64_t *)realloc(T375112_b, nc * sizeof(uint64_t));
        if (!p) abort();
        T375112_b = p;
        T375112_cap = nc;
    }
    T375112_b[++T375112_len] = v;
}

static void T375112_step(void)
{
    if (T375112_len == 0) { T375112_push(1); return; }
    uint64_t cur = T375112_b[T375112_len];
    for (uint64_t cand = 1; ; ++cand) {
        uint64_t r = cur % cand;
        if (cand >= T375112_mcap) {
            size_t nc = T375112_mcap ? T375112_mcap : 64;
            while (nc <= cand) nc *= 2;
            uint64_t **p = (uint64_t **)realloc(T375112_masks, nc * sizeof(uint64_t *));
            if (!p) abort();
            memset(p + T375112_mcap, 0, (nc - T375112_mcap) * sizeof(uint64_t *));
            T375112_masks = p;
            T375112_mcap = nc;
        }
        if (!T375112_masks[cand]) {
            T375112_masks[cand] = (uint64_t *)calloc((cand + 63) >> 6, sizeof(uint64_t));
            if (!T375112_masks[cand]) abort();
        }
        uint64_t *w = &T375112_masks[cand][r >> 6];
        uint64_t bit = 1ULL << (r & 63);
        if (!(*w & bit)) {
            *w |= bit;
            uint64_t m = T375112_len;
            T375112_push(cand);
            T375112_record(m, cand, r);
            return;
        }
    }
}

seq_t a(idx_t N)
{
    if (N == 0) return (seq_t)0;
    if (N >= T375112_tcap) {
        size_t nc = (size_t)N * 2 + 1024;
        uint64_t *p = (uint64_t *)calloc(nc, sizeof(uint64_t));
        if (!p) abort();
        free(T375112_tab);
        T375112_tab = p;
        T375112_tcap = nc;
        for (size_t m = 1; m + 1 <= T375112_len; ++m)
            T375112_record(m, T375112_b[m + 1], T375112_b[m] % T375112_b[m + 1]);
    }
    while (T375112_tab[N] == 0) T375112_step();
    return (seq_t)T375112_tab[N];
}
