/*
 * A265323  Total number of OFF (white) cells after n iterations of the "Rule
 *          110" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A265323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 303, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A265323:
   Total number of OFF cells after n iterations of Rule‑110
   starting from a single ON cell.
   The value is obtained by explicit simulation of Rule‑110. */
#include <stdlib.h>
#include <stdint.h>

/* Rule 110 truth table (binary 01101110).  Bits 7…0 correspond to
   neighborhood patterns 111…000. */
#define RULE110 110u

seq_t a(idx_t n)
{
    /* a(0) = 0 (empty row after 0 iterations) */
    if (n == 0)
        return 0;

    /* maximum length needed for any intermediate row: 2*n+1 cells */
    const size_t max_len = 2 * (size_t)n + 1;

    /* Two work buffers – they will alternate roles during the simulation. */
    uint8_t *row1 = (uint8_t *)malloc(max_len);
    uint8_t *row2 = (uint8_t *)malloc(max_len);
    if (!row1 || !row2) {
        /* In case of allocation failure, abort as the sequence cannot be
           computed. */
        abort();
    }

    /* Initialise step 0: a single ON cell. */
    row1[0] = 1;
    size_t len = 1;          /* current row length */
    size_t prev_len = 1;     /* previous row length (step 0)            */
    seq_t zeros_total = 0;   /* running total of OFF cells */

    /* Simulate steps 1 … n. */
    for (idx_t step = 1; step <= n; ++step) {
        len = prev_len + 2;        /* new row is two cells longer */
        size_t ones = 0;           /* count ON cells in the new row */

        /* Compute the new row in row2. */
        for (size_t j = 0; j < len; ++j) {
            uint8_t l = (j >= 2) ? row1[j - 2] : 0;                      /* left */
            uint8_t c = (j >= 1 && j - 1 < prev_len) ? row1[j - 1] : 0; /* centre */
            uint8_t r = (j < prev_len) ? row1[j] : 0;                     /* right */

            unsigned int pat = (l << 2) | (c << 1) | r;
            uint8_t val = (RULE110 >> pat) & 1u;
            row2[j] = val;
            ones += val;
        }

        /* Count OFF cells (zeros) in the new row. */
        size_t zeros = len - ones;
        zeros_total += (seq_t)zeros;

        /* Prepare for next iteration: swap the roles of the buffers. */
        uint8_t *tmp = row1;
        row1 = row2;
        row2 = tmp;
        prev_len = len;
    }

    free(row1);
    free(row2);
    return zeros_total;
}
