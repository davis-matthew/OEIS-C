/*
 * A267609  Decimal representation of the n-th iteration of the "Rule 182"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = decimal value of the binary row obtained after n *
 *            iterations of rule 182 on an elementary cellular automaton that
 *            * starts with a single ON cell
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes required for the implementation */
#include <stdint.h>
#include <stdlib.h>

/*
 * Compute a(n) for rule 182.
 *
 * The automaton row grows by one cell on each side each step.
 * We maintain two buffers of the maximum required width
 * (2*n + 1) and repeatedly generate the next row.
 *
 * To keep the code generic for all supported seq_t types,
 * the final decimal number is constructed by repeatedly shifting
 * a seq_t value left and adding the next bit.
 */
seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 */
    if (n == 0) {
        return (seq_t)1;               /* a(0) = 1 */
    }

    /* Maximum width after n iterations: 2*n + 1 */
    size_t max_width = (size_t)(2 * n + 1);

    /* Allocate two buffers of the maximum possible size. */
    uint8_t *row0 = (uint8_t *)calloc(max_width, sizeof(uint8_t));
    uint8_t *row1 = (uint8_t *)calloc(max_width, sizeof(uint8_t));
    if (!row0 || !row1) {
        /* Allocation failed – abort. */
        return (seq_t)0;
    }

    /* Initial row (n = 0): single ON cell */
    uint8_t *cur  = row0;      /* pointer to current row buffer   */
    uint8_t *next = row1;      /* pointer to buffer for next row */
    size_t cur_width = 1;      /* current width (2*iteration + 1) */
    cur[0] = 1;                /* a(0) starts with a single 1 */

    /* Precompute pattern values that yield 0. */
    const uint8_t ZERO_PATTERNS[3] = {0, 0b011, 0b110};

    /* Iterate from step 1 to step n, generating successive rows */
    for (idx_t step = 1; step <= n; ++step) {
        size_t next_width = cur_width + 2;      /* width after this step */

        /* Generate the next row bit by bit. */
        for (size_t i = 0; i < next_width; ++i) {
            uint8_t left, center, right;

            if (i == 0) {
                /* Leftmost new cell: neighbors 0,0,prev[0] */
                left   = 0;
                center = 0;
                right  = cur[0];
            } else if (i == next_width - 1) {
                /* Rightmost new cell: neighbors prev[cur_width-1],0,0 */
                left   = cur[cur_width - 1];
                center = 0;
                right  = 0;
            } else {
                /* Interior cells */
                /* left = (i==1) ? 0 : cur[i-2];          */
                /* center = cur[i-1];                       */
                /* right = (i==cur_width) ? 0 : cur[i];     */
                left   = (i == 1) ? 0 : cur[i - 2];
                center = cur[i - 1];
                right  = (i == cur_width) ? 0 : cur[i];
            }

            /* Encode the 3-bit pattern and test if it is a zero pattern. */
            uint8_t pat = (left << 2) | (center << 1) | right;
            /* default to 1 */
            uint8_t out = 1;
            if (pat == ZERO_PATTERNS[0] || pat == ZERO_PATTERNS[1] || pat == ZERO_PATTERNS[2]) {
                out = 0;
            }
            next[i] = out;
        }

        /* Prepare for the next iteration: swap buffers and update width. */
        uint8_t *tmp = cur;
        cur = next;
        next = tmp;
        cur_width = next_width;
    }

    /* Convert the final binary row into a seq_t value. */
    seq_t result = 0;
    for (size_t i = 0; i < cur_width; ++i) {
        result = (result << 1) | (seq_t)(cur[i]);
    }

    /* Free allocated memory. */
    free(row0);
    free(row1);

    return result;
}
