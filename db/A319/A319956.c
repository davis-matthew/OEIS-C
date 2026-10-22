/*
 * A319956  Image of 3 under repeated application of the morphism 1 -> 1, 2 ->
 *          22, 3 -> 312.
 * https://oeis.org/A319956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation of OEIS sequence A319956: 
 * a(n) is the nth symbol (0‑based) obtained by iterating the morphism
 * 1 → 1, 2 → 22, 3 → 312 starting from the single symbol 3.
 * The sequence begins 3,1,2,1,2,2,1,2,2,2,2,1,2,2,2,2,2,2,2,2,1,2,2,2,...
 * The algorithm builds the word incrementally until it reaches or exceeds
 * the requested index n.
 */

#include <stdlib.h>
#include <stdint.h>  /* for uint8_t */

seq_t a(idx_t n) {
    /* buffer that holds the current word; values are the small digits
     * 1, 2 or 3.  We use uint8_t to avoid issues with arbitrary‑precision
     * seq_t when compiled as C++ (the final value is cast back to seq_t).
     */
    size_t len = 1;                         /* current word length         */
    uint8_t *cur = (uint8_t *)malloc(4);    /* start with one symbol “3”  */
    cur[0] = 3;

    /* Expand the word until it contains an element at index n. */
    while (len <= (size_t)n) {
        size_t new_cap = len * 3 + 1;                    /* enough for the expansion */
        uint8_t *next = (uint8_t *)malloc(new_cap);
        size_t new_len = 0;

        for (size_t i = 0; i < len; ++i) {
            uint8_t s = cur[i];
            if (s == 1) {
                next[new_len++] = 1;
            } else if (s == 2) {
                next[new_len++] = 2;
                next[new_len++] = 2;
            } else { /* s == 3 */
                next[new_len++] = 3;
                next[new_len++] = 1;
                next[new_len++] = 2;
            }
        }

        free(cur);
        cur   = next;
        len   = new_len;
    }

    seq_t result = (seq_t)cur[n];
    free(cur);
    return result;
}
