/*
 * A285569  Positions of 0 in A285568; complement of A286031.
 * https://oeis.org/A285569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285568 (1-limiting word of
 *            0->11, 1->0110); found by scanning the word
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9104 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t A285568_a(idx_t n)
{
    static seq_t *w   = NULL;
    static idx_t  len = 0;
    static idx_t  iter = 0;

    if (w == NULL) {
        w   = mem_new(seq_t, 1);
        if (!w) return 0;
        w[0] = 0;
        len  = 1;
        iter = 0;
    }

    while (len < n || (iter % 2 == 0)) {
        idx_t i, pos;
        idx_t new_len = 0;
        for (i = 0; i < len; ++i) {
            seq_t c = w[i];
            new_len += (c == 0) ? 2 : 4;
        }

        seq_t *new_w = mem_new(seq_t, (size_t)new_len);
        if (!new_w) return 0;

        pos = 0;
        for (i = 0; i < len; ++i) {
            seq_t c = w[i];
            if (c == 0) {
                new_w[pos++] = 1;
                new_w[pos++] = 1;
            } else {
                new_w[pos++] = 0;
                new_w[pos++] = 1;
                new_w[pos++] = 1;
                new_w[pos++] = 0;
            }
        }

        mem_free(w);
        w   = new_w;
        len = new_len;
        ++iter;
    }

    return w[n - 1];
}

seq_t a(idx_t n)
{
    static idx_t *pos = NULL;
    static idx_t cnt = 0, cap = 0, scanned = 0;

    while (cnt < n) {
        ++scanned;
        if (A285568_a(scanned) == 0) {
            if (cnt == cap) {
                idx_t ncap = cap ? cap * 2 : 1024;
                idx_t *np = (idx_t *)realloc(pos, (size_t)ncap * sizeof(idx_t));
                if (!np) return 0;
                pos = np;
                cap = ncap;
            }
            pos[cnt++] = scanned;
        }
    }
    return (seq_t)pos[n - 1];
}
