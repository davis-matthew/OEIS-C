/*
 * A267444  Decimal representation of the middle column of the "Rule 129"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the decimal integer whose binary expansion is the
 *            successive center–cell bits of the Rule 129 elementary cellular
 *            automaton started from a single ON cell
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Base case – first term. */
    if (n == 0)
        return (seq_t)1;

    /* Width of the bit array: 2*n+1 cells are needed after n steps,
       plus one extra cell on each side to simplify boundary handling. */
    idx_t bitwidth = 2 * n + 3;          /* number of bits      */
    size_t words   = (size_t)(bitwidth + 63) / 64; /* unused, kept for clarity */
    (void)words;                        /* silence unused warning */

    /* Allocate the current and next rows; use bytes for simplicity. */
    uint8_t *cur  = (uint8_t *)calloc((size_t)bitwidth, 1);
    uint8_t *next = (uint8_t *)calloc((size_t)bitwidth, 1);
    if (!cur || !next) {
        /* If allocation fails, return zero (unlikely for n≤10000). */
        free(cur);
        free(next);
        return (seq_t)0;
    }

    /* The centre of the row is at offset n+1.  This gives n+1 cells
       to the left and to the right of the centre. */
    idx_t centre = n + 1;
    cur[centre] = 1;    /* initial single ON cell */

    seq_t result = (seq_t)1;   /* a(0) */

    /* Iterate from step 1 to step n.  At each step, compute the
       next state and record the new centre bit. */
    for (idx_t step = 1; step <= n; ++step) {
        for (idx_t i = 0; i < bitwidth; ++i) {
            uint8_t left  = (i == 0)      ? 0 : cur[i - 1];
            uint8_t mid   = cur[i];
            uint8_t right = (i == bitwidth - 1) ? 0 : cur[i + 1];

            /* Rule 129: output 1 iff all three neighbours are equal. */
            next[i] = (left == mid && mid == right) ? 1 : 0;
        }

        /* Append the new centre bit to the result. */
        result = (result << 1) | (seq_t)next[centre];

        /* Prepare for the next iteration. */
        uint8_t *tmp = cur;
        cur = next;
        next = tmp;
        memset(next, 0, (size_t)bitwidth);
    }

    free(cur);
    free(next);
    return result;
}
