/*
 * A284677  1-limiting word of the morphism 0->1, 1-> 0111.
 * https://oeis.org/A284677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1427, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 1-limiting word of the morphism 0->1, 1->0111.  The function returns
   the nth term (n ≥ 1) of the infinite word that results from iterating
   the morphism starting from the symbol 0 with a *odd* number of
   iterations. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static tables of length of the expansion of 0 and 1 after d
       iterations.  len0[d] is the length starting from symbol 0 after d
       iterations; len1[d] is the same starting from symbol 1.  The tables
       are grown as needed. */
    static seq_t *len0 = NULL;
    static seq_t *len1 = NULL;
    static size_t cap   = 0;   /* capacity of the tables */
    static size_t depth = 0;   /* largest depth that has been computed */

    if (len0 == NULL) {
        /* initialise small tables */
        cap = 4;
        len0 = (seq_t *)malloc(cap * sizeof(seq_t));
        len1 = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!len0 || !len1) return (seq_t)0; /* allocation failure */
        len0[0] = 1;
        len1[0] = 1;
        depth = 0;
    }

    /* Grow tables until len0[depth] >= n.  The requirement is to use an
       odd number of iterations, so we grow at least to the first odd
       depth that satisfies the bound. */
    while (len0[depth] < (seq_t)n) {
        if (depth + 1 >= cap) {
            cap <<= 1;
            len0 = (seq_t *)realloc(len0, cap * sizeof(seq_t));
            len1 = (seq_t *)realloc(len1, cap * sizeof(seq_t));
            if (!len0 || !len1) return (seq_t)0;
        }
        /* recurrence for lengths:
           len0[d] = len1[d-1]
           len1[d] = len0[d-1] + 3 * len1[d-1] */
        len0[depth + 1] = len1[depth];
        len1[depth + 1] = len0[depth] + (seq_t)3 * len1[depth];
        depth++;
    }

    /* If depth is even, use the next odd depth (the length will be
       strictly larger). */
    if ((depth & 1) == 0) {
        if (depth + 1 >= cap) {
            cap <<= 1;
            len0 = (seq_t *)realloc(len0, cap * sizeof(seq_t));
            len1 = (seq_t *)realloc(len1, cap * sizeof(seq_t));
            if (!len0 || !len1) return (seq_t)0;
        }
        len0[depth + 1] = len1[depth];
        len1[depth + 1] = len0[depth] + (seq_t)3 * len1[depth];
        depth++;   /* now depth is odd */
    }

    /* Descend from the root symbol 0 under the chosen odd depth. */
    seq_t cur = 0;          /* starting symbol */
    seq_t idx = (seq_t)n;   /* 1‑based position within the word */
    size_t d   = depth;

    while (d > 0) {
        if (cur == 0) {
            /* 0 expands to a single 1 */
            cur = 1;
            d--;
        } else { /* cur == 1 */
            seq_t l0 = len0[d - 1];
            seq_t l1 = len1[d - 1];
            if (idx <= l0) {
                cur = 0;
                d--;
            } else if (idx <= l0 + l1) {
                idx -= l0;
                /* cur stays 1 */
                d--;
            } else if (idx <= l0 + 2 * l1) {
                idx -= l0 + l1;
                /* cur stays 1 */
                d--;
            } else {
                idx -= l0 + 2 * l1;
                /* cur stays 1 */
                d--;
            }
        }
    }

    return cur;
}
