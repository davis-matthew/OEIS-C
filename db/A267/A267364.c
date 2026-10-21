/*
 * A267364  Binary representation of the n-th iteration of the "Rule 126"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267364
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

/* Rule 126 cellular automaton – binary pattern of the n‑th iteration
   starting with a single ON cell.  The returned value is the decimal integer
   whose decimal digits are the pattern bits (most significant bit first); */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Pattern length after n iterations is 2*n+1. */
    size_t maxlen = (size_t)(2 * n + 1);

    /* Allocate two working rows. */
    uint8_t *cur  = (uint8_t*)malloc(maxlen * sizeof(uint8_t));
    uint8_t *next = (uint8_t*)malloc(maxlen * sizeof(uint8_t));
    if (!cur || !next) {
        free(cur); free(next);
        return (seq_t)0;   /* should not happen in tests */
    }

    /* Initialise the first row with a single ON cell. */
    for (size_t i = 0; i < maxlen; ++i) cur[i] = 0;
    cur[0] = 1;

    /* Perform Rule 126 iterations. */
    for (idx_t step = 1; step <= n; ++step) {
        size_t oldsize = (size_t)(2 * (step - 1) + 1);
        size_t newsize = oldsize + 2;

        for (size_t i = 0; i < newsize; ++i) {
            uint8_t left   = (i >= 2 && (i - 2) < oldsize) ? cur[i - 2] : 0;
            uint8_t center = (i >= 1 && (i - 1) < oldsize) ? cur[i - 1] : 0;
            uint8_t right  = (i < oldsize) ? cur[i] : 0;
            /* Rule 126: the new cell is 0 only for 000 or 111; otherwise 1. */
            next[i] = (left == center && center == right) ? 0 : 1;
        }
        /* Swap working rows. */
        uint8_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Convert the final pattern to a decimal integer
       (most significant bit first). */
    seq_t result = 0;
    for (size_t i = 0; i < maxlen; ++i) {
        result = result * 10 + (seq_t)cur[i];
    }

    free(cur); free(next);
    return result;
}
