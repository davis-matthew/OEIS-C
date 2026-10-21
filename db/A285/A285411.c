/*
 * A285411  0-limiting word of the morphism 0->11, 1-> 010.
 * https://oeis.org/A285411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0-limiting word of the morphism 0->11, 1->010.
   Equivalent to the fixed point of the composite morphism
   g: 0 -> 010010 (length 6), 1 -> 1101011 (length 7). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset: a(1) is the first term. */
    if (n <= 1) {
        return (seq_t)0;
    }

    size_t idx = (size_t)(n - (idx_t)1);     /* 0‑based index */
    size_t len = 1;
    seq_t *cur = (seq_t *)calloc(len, sizeof(seq_t));   /* initial word "0" */
    if (!cur) return (seq_t)0;     /* allocation failure – unlikely */
    cur[0] = (seq_t)0;

    /* Iterate the morphism g until the word is long enough. */
    while (len <= idx) {
        /* Compute length of the next word. */
        size_t newlen = 0;
        for (size_t i = 0; i < len; ++i) {
            newlen += (cur[i] == (seq_t)0) ? 6 : 7;
        }

        seq_t *next = (seq_t *)calloc(newlen, sizeof(seq_t));
        if (!next) {
            free(cur);
            return (seq_t)0;     /* allocation failure */
        }

        /* Expand each symbol according to g. */
        size_t pos = 0;
        for (size_t i = 0; i < len; ++i) {
            if (cur[i] == (seq_t)0) {
                /* g(0) = 010010 */
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
            } else {
                /* g(1) = 1101011 */
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)1;
            }
        }

        free(cur);
        cur = next;
        len = newlen;
    }

    seq_t result = cur[idx];
    free(cur);
    return result;
}
