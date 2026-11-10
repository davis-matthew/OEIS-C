/*
 * A285140  Positions of 0 in A285139; complement of A285141.
 * https://oeis.org/A285140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285139, the 0-limiting word of
 *            the morphism 0->10, 1->0010 (computed by iterating the morphism
 *            and scanning)
 * Range:     u8 n <= 161, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9914 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A285139_a(idx_t n)
{
    static seq_t *buf = NULL;
    static size_t len = 0;
    static idx_t iter = 0;

    if (len == 0) {
        buf = mem_new(seq_t, 1);
        if (buf == NULL) return (seq_t)0;
        buf[0] = (seq_t)0;
        len = (size_t)1;
        iter = 0;
    }

    while ((len < n) || ((iter & (idx_t)1) != 0)) {
        size_t new_len = 0;
        for (size_t i = 0; i < len; ++i)
            new_len += (buf[i] == (seq_t)0) ? 2 : 4;

        seq_t *new_buf = mem_new(seq_t, new_len);
        if (new_buf == NULL) return (seq_t)0;
        size_t p = 0;
        for (size_t i = 0; i < len; ++i) {
            if (buf[i] == (seq_t)0) {
                new_buf[p++] = (seq_t)1;
                new_buf[p++] = (seq_t)0;
            } else {
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

seq_t a(idx_t n)
{
    idx_t limit = 2 * n + 10;
    idx_t count = 0;
    idx_t pos;
    A285139_a(limit); /* ensure the word is long enough */
    for (pos = 1; pos <= limit; ++pos) {
        if (A285139_a(pos) == (seq_t)0) {
            ++count;
            if (count == n) return (seq_t)pos;
        }
    }
    return (seq_t)0;
}
