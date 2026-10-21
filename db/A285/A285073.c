/*
 * A285073  0-limiting word of the morphism 0->10, 1-> 010.
 * https://oeis.org/A285073
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

/* Computes a(n) for OEIS A285073.
 * The sequence is the 0‑limiting word of the morphism
 *   0 -> 10, 1 -> 010.
 * The word is obtained by iterating the morphism from the seed 0
 * and selecting the result after an even number of iterations.
 * This implementation builds the word incrementally until the
 * requested index lies within a word that comes from an even
 * iteration.  All symbols are stored as 0/1 bytes; the function
 * returns the requested symbol cast to seq_t. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state retained between calls */
    static uint8_t *cur = NULL;   /* current word (bytes 0/1) */
    static size_t   cur_len = 0;  /* length of cur */
    static size_t   iteration = 0;/* number of performed expansions */

    /* initialise with the seed word '0' (iteration 0) */
    if (cur == NULL) {
        cur = (uint8_t *)malloc(1);
        cur[0] = 0;
        cur_len = 1;
        iteration = 0;
    }

    /* keep expanding until length is large enough and the
       word comes from an even iteration (0‑word).  The offset
       in the problem is 1, so we treat n as 1‑based.  */
    while ((cur_len < (size_t)n) || (iteration & 1)) {
        size_t zeros = 0, ones = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            if (cur[i] == 0)
                ++zeros;
            else
                ++ones;
        }

        size_t new_len = zeros * 2 + ones * 3;
        uint8_t *next = (uint8_t *)malloc(new_len);
        size_t pos = 0;

        for (size_t i = 0; i < cur_len; ++i) {
            if (cur[i] == 0) {        /* 0 → 10 */
                next[pos++] = 1;
                next[pos++] = 0;
            } else {                  /* 1 → 010 */
                next[pos++] = 0;
                next[pos++] = 1;
                next[pos++] = 0;
            }
        }

        free(cur);
        cur = next;
        cur_len = new_len;
        ++iteration;
    }

    /* cast the byte value to seq_t */
    return (seq_t)cur[n - 1];
}
