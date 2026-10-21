/*
 * A284490  1-limiting word of the morphism 0->1, 1->0011.
 * https://oeis.org/A284490
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

/* 1‑limiting word of the morphism 0->1, 1->0011.
    a(n) returns the nth symbol (1 or 0) of the infinite word
    obtained by iterating the morphism an odd number of times
    so that the word starts with a '1'.
    The function computes the word iteratively until the
    required position is within the current word. */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined but safe */

    /* current word, stored as characters '0'/'1' */
    char *cur = (char *)malloc(1);
    cur[0] = '0';
    size_t cur_len = 1;              /* length of cur */
    idx_t target = n;                /* target position (1‑based) */
    idx_t level = 0;                 /* current iteration number */

    /* iterate until we reach an odd level with length >= target */
    while ( ((level & 1ULL) == 0) || (cur_len < target) ) {
        size_t cap = cur_len * 4;    /* maximum possible length next */
        char *next = (char *)malloc(cap);
        size_t nxt_len = 0;

        for (size_t i = 0; i < cur_len; ++i) {
            if (cur[i] == '0') {
                next[nxt_len++] = '1';
            } else { /* cur[i] == '1' */
                next[nxt_len++] = '0';
                next[nxt_len++] = '0';
                next[nxt_len++] = '1';
                next[nxt_len++] = '1';
            }
        }

        free(cur);
        cur = next;
        cur_len = nxt_len;
        ++level;                    /* next iteration */
    }

    /* get the required symbol and free memory */
    char val = cur[target - 1];      /* 0‑based index */
    free(cur);
    return (seq_t)(val - '0');       /* return 0 or 1 as seq_t */
}
