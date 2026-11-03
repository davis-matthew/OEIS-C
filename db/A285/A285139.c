/*
 * A285139  0-limiting word of the morphism 0->10, 1-> 0010.
 * https://oeis.org/A285139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 8778 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0-limiting word of the morphism 0->10, 1->0010 (even iterations) */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of the current word, its length, and the iteration
       number (0‑based). */
    static seq_t *buf = NULL;
    static size_t len = 0;
    static idx_t iter = 0;

    /* initialise on first call */
    if (len == 0) {
        buf = mem_new(seq_t, 1);
        if (buf == NULL) return (seq_t)0;  /* safety, should never happen */
        buf[0] = (seq_t)0;
        len = (size_t)1;
        iter = 0;
    }

    /* iterate until the word has length at least n and the
       iteration count is even (the 0‑limiting word). */
    while ((len < n) || ((iter & (idx_t)1) != 0)) {
        size_t new_len = 0;
        for (size_t i = 0; i < len; ++i)
            new_len += (buf[i] == (seq_t)0) ? 2 : 4;

        seq_t *new_buf = mem_new(seq_t, new_len);
        if (new_buf == NULL) return (seq_t)0;  /* safety */
        size_t p = 0;
        for (size_t i = 0; i < len; ++i) {
            if (buf[i] == (seq_t)0) {           /* 0 → "10" */
                new_buf[p++] = (seq_t)1;
                new_buf[p++] = (seq_t)0;
            } else {                            /* 1 → "0010" */
                new_buf[p++] = (seq_t)0;
                new_buf[p++] = (seq_t)0;
                new_buf[p++] = (seq_t)1;
                new_buf[p++] = (seq_t)0;
            }
        }
        mem_free(buf);
        buf = new_buf;
        len = new_len;
        ++iter;
    }

    return buf[n - 1];
}
