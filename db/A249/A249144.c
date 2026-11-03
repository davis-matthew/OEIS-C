/*
 * A249144  a(0) = 0, after which a(n) gives the total number of runs of the
 *          same length as the rightmost run in the binary representation of
 *          a(n-1) [i.e., A136480(a(n-1))] among the binary expansions of all
 *          previous terms, including the runs in a(n-1) itself.
 * https://oeis.org/A249144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9797 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n>0, let l be the length of the rightmost run of equal bits
   in the binary representation of a(n-1); then a(n) = total number of runs of
   length l among the binary expansions of a(0)..a(n-1). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* add all runs of value v to the counts table */
static void add_runs(seq_t v, uint64_t *counts, size_t *cap)
{
    if (v == (seq_t)0) {
        if (1 >= *cap) {
            size_t newcap = (*cap) * 2 < 2 ? 2 : (*cap) * 2;
            uint64_t *nc = (uint64_t *)realloc(counts, newcap * sizeof(uint64_t));
            memset(nc + *cap, 0, (newcap - *cap) * sizeof(uint64_t));
            counts = nc;
            *cap = newcap;
        }
        counts[1] += 1;
        return;
    }
    bool cur = (bool)(v & (seq_t)1);
    size_t runlen = 1;
    seq_t t = v >> 1;
    while (t != (seq_t)0) {
        bool next = (bool)(t & (seq_t)1);
        if (next == cur) {
            ++runlen;
        } else {
            if (runlen >= *cap) {
                size_t newcap = (*cap) * 2;
                if (newcap <= runlen) newcap = runlen + 1;
                uint64_t *nc = (uint64_t *)realloc(counts, newcap * sizeof(uint64_t));
                memset(nc + *cap, 0, (newcap - *cap) * sizeof(uint64_t));
                counts = nc;
                *cap = newcap;
            }
            ++counts[runlen];
            cur = next;
            runlen = 1;
        }
        t >>= 1;
    }
    if (runlen >= *cap) {
        size_t newcap = (*cap) * 2;
        if (newcap <= runlen) newcap = runlen + 1;
        uint64_t *nc = (uint64_t *)realloc(counts, newcap * sizeof(uint64_t));
        memset(nc + *cap, 0, (newcap - *cap) * sizeof(uint64_t));
        counts = nc;
        *cap = newcap;
    }
    ++counts[runlen];
}

/* compute rightmost run length of binary representation of v */
static size_t runlen_rightmost(seq_t v)
{
    if (v == (seq_t)0) return 1;
    bool b = (bool)(v & (seq_t)1);
    size_t l = 1;
    seq_t t = v >> 1;
    while (t != (seq_t)0) {
        if ((bool)(t & (seq_t)1) != b) break;
        ++l;
        t >>= 1;
    }
    return l;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* static memoization storage */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;

    static uint64_t *counts = NULL;
    static size_t counts_cap = 0;

    /* initialise once */
    if (!seq) {
        seq_cap = 4;
        seq = mem_new(seq_t, seq_cap);
        seq_len = 1;
        seq[0] = (seq_t)0;

        counts_cap = 16;
        counts = (uint64_t *)calloc(counts_cap, sizeof(uint64_t));
        counts[1] = 1;          /* runs in the term 0 */
    }

    /* compute up to requested index if necessary */
    if ((idx_t)seq_len - 1 < n) {
        for (idx_t i = seq_len; i <= n; ++i) {
            /* rightmost run length of previous term */
            size_t l = runlen_rightmost(seq[i-1]);

            /* ensure counts capacity for l */
            if (l >= counts_cap) {
                size_t newcap = counts_cap * 2;
                if (newcap <= l) newcap = l + 1;
                uint64_t *nc = (uint64_t *)realloc(counts, newcap * sizeof(uint64_t));
                memset(nc + counts_cap, 0, (newcap - counts_cap) * sizeof(uint64_t));
                counts = nc;
                counts_cap = newcap;
            }

            /* compute current term */
            seq_t val = (seq_t)counts[l];

            /* ensure seq capacity */
            if ((size_t)i >= seq_cap) {
                size_t newcap = seq_cap * 2;
                if (newcap <= (size_t)i) newcap = (size_t)i + 1;
                seq = mem_resize(seq_t, seq, newcap);
                seq_cap = newcap;
            }
            seq[i] = val;

            /* add runs of new term to counts */
            add_runs(val, counts, &counts_cap);

            seq_len = (size_t)i + 1;
        }
    }

    return seq[n];
}
