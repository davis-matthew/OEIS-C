/*
 * A270811  Records in A266569.
 * https://oeis.org/A270811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A266569(A270812(n)); records of A266569 (a(1)=1;
 *            a(2k)=4k+a(k); a(2k+1)=k+a(4k+4)), found by scanning indices
 *            upward
 * Range:     u8 n <= 7, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 919 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A266569_a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if ((n & 1) == 0) {
        idx_t k = n >> 1;
        return (seq_t)4 * (seq_t)k + A266569_a(k);
    } else {
        idx_t k = (n - 1) >> 1;
        return (seq_t)k + A266569_a(4 * k + 4);
    }
}

static seq_t *recs = NULL;
static idx_t rec_cnt = 0, rec_cap = 0;
static idx_t scan_pos = 0;
static seq_t cur_max = 0;

static void push_rec(seq_t v)
{
    if (rec_cnt == rec_cap) {
        idx_t nc = rec_cap ? rec_cap * 2 : 64;
        seq_t *p = mem_new(seq_t, nc);
        for (idx_t i = 0; i < rec_cnt; ++i) p[i] = recs[i];
        if (recs) mem_free(recs);
        recs = p;
        rec_cap = nc;
    }
    recs[rec_cnt++] = v;
}

seq_t a(idx_t n)
{
    while (rec_cnt < n) {
        ++scan_pos;
        seq_t v = A266569_a(scan_pos);
        if (scan_pos == 1 || v > cur_max) {
            cur_max = v;
            push_rec(v);
        }
    }
    return recs[n - 1];
}
