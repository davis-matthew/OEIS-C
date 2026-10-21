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
 * Helpers:   add_runs, runlen_rightmost
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n>0, let l be the length of the rightmost run of equal bits
   in the binary representation of a(n-1); then a(n) = total number of runs of
   length l among the binary expansions of a(0)..a(n-1). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/add_runs.h"
#include "../utils/runlen_rightmost.h"

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
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
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
                seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
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
