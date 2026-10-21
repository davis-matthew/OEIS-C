/*
 * A266809  Binary representation of the n-th iteration of the "Rule 62"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266809
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

/* Binary representation of the nth iteration of "Rule 62" elementary
   cellular automaton starting with a single ON (black) cell.
   a(n) = A007088(A266810(n)). */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Maximum number of cells after n steps: 2 n + 1.  */
    const size_t maxlen = (size_t)(2 * n + 1);

    /* Allocate two buffers for the current and next rows.  */
    uint8_t *cur  = (uint8_t *)calloc(maxlen, 1);
    uint8_t *next = (uint8_t *)malloc(maxlen);
    if (!cur || !next) {
        /* Allocation failure – return zero as a sentinel.  */
        return (seq_t)0u;
    }

    /* Start configuration: single ON cell.  */
    cur[0] = 1u;
    size_t len = 1;  /* number of active cells in cur. */

    /* Rule 62 in binary: 00111110 (8‑bit rule table).  */
    const uint8_t rule = 62u;

    /* Perform n iterations.  */
    for (idx_t step = 0; step < n; ++step) {
        const size_t next_len = len + 2;

        for (size_t k = 0; k < next_len; ++k) {
            uint8_t left  = (k >= 2)   ? cur[k - 2] : 0u;
            uint8_t mid   = (k >= 1 && k - 1 < len) ? cur[k - 1] : 0u;
            uint8_t right = (k < len)  ? cur[k] : 0u;
            uint8_t idx   = (left << 2) | (mid << 1) | right;
            next[k] = (rule >> idx) & 1u;
        }

        /* Swap buffers and update the length.  */
        uint8_t *tmp = cur;
        cur = next;
        next = tmp;
        len = next_len;
    }

    /* Convert the final binary string to a decimal number whose
       digits are the bits of the pattern.  */
    seq_t result = (seq_t)0u;
    for (size_t i = 0; i < len; ++i) {
        result = (result * (seq_t)10) + (seq_t)cur[i];
    }

    /* Clean up.  */
    free(cur);   /* cur holds the final row. */
    free(next);  /* next holds the previous row. */

    return result;
}
