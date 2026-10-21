/*
 * A285341  Fixed point of the morphism 0 -> 10, 1 -> 1011.
 * https://oeis.org/A285341
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

/* Sequence A285341: a(n) is the n‑th term of the fixed point of the
   morphism 0 → 10, 1 → 1011 (starting from the seed word 0). */

/* The fixed point is the limit of applying the morphism repeatedly.
   To obtain the n‑th bit we iteratively expand the current word
   until its length exceeds n, then return the (n‑1)‑st element. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* n is 1‑based; if n < 1 the behaviour is undefined but
       we guard against zero to keep the function defined. */
    if (n == 0) return (seq_t)0;

    /* Initially the word is the seed "0". */
    idx_t len = 1;
    uint8_t *cur = (uint8_t *)malloc(1);
    cur[0] = 0;            /* seed word “0” */

    /* Perform expansions until the word length exceeds n. */
    while (len <= n) {
        /* Determine length after the next morphism application. */
        idx_t new_len = 0;
        for (idx_t i = 0; i < len; ++i)
            new_len += (cur[i] == 0) ? 2 : 4;   /* 0 → 2, 1 → 4 */

        /* Allocate space for the next word. */
        uint8_t *next = (uint8_t *)malloc(new_len);
        idx_t pos = 0;
        for (idx_t i = 0; i < len; ++i) {
            if (cur[i] == 0) {
                /* “10” */
                next[pos++] = 1;
                next[pos++] = 0;
            } else {
                /* “1011” */
                next[pos++] = 1;
                next[pos++] = 0;
                next[pos++] = 1;
                next[pos++] = 1;
            }
        }

        /* Dispose of the previous word and update pointers. */
        free(cur);
        cur = next;
        len = new_len;
    }

    /* Extract the n‑th term (index n‑1). */
    seq_t result = (seq_t)cur[n - 1];
    free(cur);
    return result;
}
