/*
 * A284751  Fixed point of the morphism 0 -> 01, 1 -> 0001.
 * https://oeis.org/A284751
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

/* A284751: Fixed point of the morphism 0 → 01, 1 → 0001
   Compute the n-th symbol (1‑based).  No external dependencies are used. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0U)   /* undefined for n = 0, return 0 for safety */
        return 0U;

    /* -------- determine the number of morphism applications -------- */
    size_t target = (size_t)n;   /* n fits in a size_t for the loop bounds */
    size_t len   = 1;            /* current total length of the word   */
    size_t z     = 1;            /* number of 0's in the current word   */
    size_t o     = 0;            /* number of 1's in the current word   */
    size_t level = 0;

    /* grow until the word is long enough */
    while (len < target) {
        size_t z_next = z + 3 * o;   /* 0 → 01 produces one 0 and one 1
                                         1 → 0001 produces three 0's and one 1 */
        size_t o_next = z + o;       /* 0 → 01 produces one 1
                                         1 → 0001 produces one 1        */
        z = z_next;
        o = o_next;
        len = z + o;                /* new total length          */
        ++level;
    }

    /* -------- allocate two buffers to hold the word at the current level -------- */
    uint8_t *buf1 = (uint8_t *)malloc(len);
    uint8_t *buf2 = (uint8_t *)malloc(len);
    if (!buf1 || !buf2) {          /* allocation failed */
        free(buf1);
        free(buf2);
        return 0U;
    }

    /* -------- initialise the word with the initial symbol 0 -------- */
    buf1[0] = 0;
    uint8_t *cur  = buf1;
    uint8_t *next = buf2;
    size_t cur_len = 1;

    /* -------- apply the morphism 'level' times -------- */
    for (size_t l = 0; l < level; ++l) {
        size_t pos = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            if (cur[i] == 0) {          /* 0 → 01 */
                next[pos++] = 0;
                next[pos++] = 1;
            } else {                    /* 1 → 0001 */
                next[pos++] = 0;
                next[pos++] = 0;
                next[pos++] = 0;
                next[pos++] = 1;
            }
        }
        cur_len = pos;                 /* new length after this expansion */

        /* swap buffers for the next iteration */
        uint8_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* -------- fetch the requested symbol and free memory -------- */
    seq_t result = (seq_t)cur[(size_t)n - 1U];
    free(buf1);
    free(buf2);
    return result;
}
