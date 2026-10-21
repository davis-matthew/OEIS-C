/*
 * A288711  1-limiting word of the mapping 00->1000, 10->00, starting with 00.
 * https://oeis.org/A288711
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

/*  Sequence A288711: 1‑limiting word of the mapping 00→1000, 10→00,
    starting with 00.  The algorithm expands the word until the
    requested index is inside the current word and returns its bit. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static idx_t cur_len = 0;          /* current word length           */
    static uint8_t *cur = NULL;       /* current word buffer           */
    static uint8_t *next = NULL;      /* buffer for next iteration    */
    static int initialized = 0;       /* flag whether buffers exist    */

    /* initialise once with the first iterate: 1000 */
    if (!initialized) {
        cur_len = 4;
        cur = (uint8_t *)malloc(cur_len);
        cur[0] = 1; cur[1] = 0; cur[2] = 0; cur[3] = 0;
        next = (uint8_t *)malloc(cur_len * 2);  /* room for next step   */
        initialized = 1;
    }

    if (n == 0)   /* undefined; return 0 as a safe default */
        return (seq_t)0;

    /* extend the word until it contains the n-th bit */
    while (cur_len < n) {
        idx_t new_len = 0;
        for (idx_t i = 0; i < cur_len; i += 2) {
            /* every pair is either 00 or 10 */
            if (cur[i] == 0 && cur[i + 1] == 0) {
                /* 00 → 1000 */
                next[new_len++] = 1;
                next[new_len++] = 0;
                next[new_len++] = 0;
                next[new_len++] = 0;
            } else {            /* 10 → 00 */
                next[new_len++] = 0;
                next[new_len++] = 0;
            }
        }
        free(cur);
        cur = next;
        cur_len = new_len;
        next = (uint8_t *)malloc(cur_len * 2);
    }

    return (seq_t)cur[n - 1];
}
