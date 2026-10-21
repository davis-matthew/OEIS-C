/*
 * A266381  Binary representation of the n-th iteration of the "Rule 22"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rule‑22 cellular automaton:
   a(n) is the decimal integer whose decimal digits are the
   binary cells of the n‑th generation of a Rule‑22 automaton
   starting from a single ON cell.  */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Rule‑22 lookup table: 111→0, 110→0, 101→0, 100→1,
       011→0, 010→1, 001→1, 000→0. */
    static const unsigned char RULE[8] =
    { 0, 1, 1, 0, 1, 0, 0, 0 };

    /* Allocate two buffers large enough for the longest row. */
    size_t array_len = (size_t)n * 2 + 5;   /* 2*n+1 + margin */
    unsigned char *cur = (unsigned char *)calloc(array_len, sizeof(unsigned char));
    unsigned char *nxt = (unsigned char *)calloc(array_len, sizeof(unsigned char));
    if (!cur || !nxt) {
        /* Allocation failure – clean up and return 0. */
        free(cur);
        free(nxt);
        return 0;
    }

    size_t len = 1;       /* current row length (2*step+1) */
    cur[0] = 1;           /* generation 0 : "1" */

    for (idx_t step = 0; step < n; ++step) {
        size_t new_len = len + 2;   /* next row is 2 longer */

        for (size_t k = 0; k < new_len; ++k) {
            unsigned left  = (k >= 2 && (k - 2) < len) ? cur[k - 2] : 0;
            unsigned mid   = (k >= 1 && (k - 1) < len) ? cur[k - 1] : 0;
            unsigned right = (k < len) ? cur[k] : 0;
            unsigned idx   = (left << 2) | (mid << 1) | right;
            nxt[k] = RULE[idx];
        }

        /* swap buffers for next iteration */
        unsigned char *tmp = cur;
        cur  = nxt;
        nxt  = tmp;
        len = new_len;
    }

    /* Convert the final bit pattern to a seq_t integer whose decimal
       representation consists of the binary digits of the row. */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * 10 + (seq_t)cur[i];
    }

    free(cur);
    free(nxt);
    return result;
}
