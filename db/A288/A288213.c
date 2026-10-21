/*
 * A288213  Fixed point of the mapping 00->0010, 1->011, starting with 00.
 * https://oeis.org/A288213
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

/* Sequence A288213: fixed point of the morphism
 * 00 → 0010,
 * 1  → 011
 * starting from "00".  The function returns the value a(n)
 * for n ≥ 1 (a(1) is the first term).  It iteratively expands
 * the fixed point until at least n bits are available. */  

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* static state preserves the current fixed‑point prefix */
    static unsigned char *cur = NULL;   /* current sequence bits */
    static size_t cur_len = 0;          /* length of current sequence */

    /* initialise with the seed "00" */
    if (cur == NULL) {
        cur_len = 2;
        cur = (unsigned char *)malloc(cur_len);
        cur[0] = 0;
        cur[1] = 0;
    }

    /* guard against a zero index – not required by the spec but safe */
    if (n == 0)
        return (seq_t)0;

    /* expand until we have at least n bits */
    while (cur_len < (size_t)n) {
        size_t i, new_len = 0;

        /* first pass: compute the length of the next sequence */
        for (i = 0; i < cur_len; ++i) {
            if (cur[i] == 0) {
                if ((i + 1 < cur_len) && cur[i + 1] == 0) {
                    new_len += 4;   /* 00 → 0010 */
                    ++i;            /* skip the second 0 of the pair */
                } else {
                    new_len += 1;   /* isolated 0 stays 0 */
                }
            } else { /* cur[i] == 1 */
                new_len += 3;       /* 1 → 011 */
            }
        }

        /* allocate buffer for the next sequence */
        unsigned char *next = (unsigned char *)malloc(new_len);
        size_t j = 0;

        /* second pass: build the new sequence */
        for (i = 0; i < cur_len; ++i) {
            if (cur[i] == 0) {
                if ((i + 1 < cur_len) && cur[i + 1] == 0) {
                    next[j++] = 0;
                    next[j++] = 0;
                    next[j++] = 1;
                    next[j++] = 0;
                    ++i; /* skip the second 0 of the pair */
                } else {
                    next[j++] = 0;
                }
            } else { /* cur[i] == 1 */
                next[j++] = 0;
                next[j++] = 1;
                next[j++] = 1;
            }
        }

        /* replace the old sequence */
        free(cur);
        cur = next;
        cur_len = new_len;
    }

    return (seq_t)cur[(size_t)(n - 1)];
}
