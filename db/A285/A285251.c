/*
 * A285251  Positions of 1 in A285249; complement of A285250.
 * https://oeis.org/A285251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A285249 (0-limiting word of
 *            0->10, 1->0101); conjecture 2n - a(n) in {0,1}
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9252 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A285249_a(idx_t n)
{
    static seq_t *seq   = NULL;
    static idx_t  seq_len = 0;
    static idx_t  iter    = 0;

    if (seq == NULL) {
        seq = mem_new(seq_t, 1);
        seq[0] = (seq_t)0;
        seq_len = 1;
    }

    while (1) {
        if (seq_len >= n && ((iter & 1U) == 0))
            return seq[(size_t)(n - 1)];

        idx_t ones = 0;
        for (idx_t i = 0; i < seq_len; ++i)
            if (seq[i] == (seq_t)1)
                ++ones;

        idx_t new_len = seq_len * 2 + ones * 2;

        seq_t *new_seq = mem_new(seq_t, (size_t)new_len);
        idx_t pos = 0;

        for (idx_t i = 0; i < seq_len; ++i) {
            if (seq[i] == (seq_t)0) {
                new_seq[pos++] = (seq_t)1;
                new_seq[pos++] = (seq_t)0;
            } else {
                new_seq[pos++] = (seq_t)0;
                new_seq[pos++] = (seq_t)1;
                new_seq[pos++] = (seq_t)0;
                new_seq[pos++] = (seq_t)1;
            }
        }

        mem_free(seq);
        seq      = new_seq;
        seq_len  = new_len;
        ++iter;
    }
}

seq_t a(idx_t n)
{
    static idx_t *pos = NULL;
    static idx_t cnt = 0, cap = 0, scanned = 0;

    while (cnt < n) {
        ++scanned;
        if (A285249_a(scanned) == (seq_t)1) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                pos = (idx_t *)realloc(pos, (size_t)cap * sizeof(idx_t));
            }
            pos[cnt++] = scanned;
        }
    }
    return (seq_t)pos[n - 1];
}
