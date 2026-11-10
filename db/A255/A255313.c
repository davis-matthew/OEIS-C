/*
 * A255313  Triangle read by rows: row n contains the sums of adjacent pairs
 *          of terms in row n of A088643.
 * https://oeis.org/A255313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A088643(n+1,k) + A088643(n+1,k+1), i.e. adjacent sums
 *            in row n+1 of A088643 (rows built by greedy search), flattened;
 *            T(n,1) = A060265(n+1)
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Validated: standard, pass, 89 terms through n = 7828 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *t_vals = NULL;
static size_t t_len = 0;
static size_t t_cap = 0;
static unsigned long next_m = 2;

static void t_push(seq_t v)
{
    if (t_len == t_cap) {
        size_t nc = t_cap ? t_cap * 2 : 64;
        seq_t *p = mem_resize(seq_t, t_vals, nc);
        if (!p) exit(1);
        t_vals = p;
        t_cap = nc;
    }
    t_vals[t_len++] = v;
}

static void t_gen_row(unsigned long m)
{
    size_t lim = (size_t)m * 2 + 2;
    unsigned char *comp = (unsigned char *)calloc(lim + 1, 1);
    unsigned char *used = (unsigned char *)calloc((size_t)m + 1, 1);
    if (!comp || !used) exit(1);
    comp[0] = 1;
    comp[1] = 1;
    for (size_t i = 2; i * i <= lim; ++i)
        if (!comp[i])
            for (size_t j = i * i; j <= lim; j += i) comp[j] = 1;

    unsigned long prev = m;
    for (;;) {
        long cand = -1;
        for (long j = (long)m - 1; j >= 1; --j) {
            if (!used[j] && !comp[prev + (unsigned long)j]) { cand = j; break; }
        }
        if (cand < 0) break;
        used[cand] = 1;
        t_push((seq_t)(prev + (unsigned long)cand));
        prev = (unsigned long)cand;
    }
    free(comp);
    free(used);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while ((idx_t)t_len < n) {
        t_gen_row(next_m);
        ++next_m;
    }
    return t_vals[(size_t)(n - 1)];
}
