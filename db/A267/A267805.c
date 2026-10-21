/*
 * A267805  Decimal representation of the n-th iteration of the "Rule 214"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = decimal representation of the n‑th iteration of Rule 214
 *            * elementary cellular automaton starting with a single ON cell
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Rule 214 table:  index = pattern (bits L C R) as integer 0..7,
   value = new state (0 or 1). */
static const uint8_t rule_table[8] =
{
    0, /* 000 */
    1, /* 001 */
    1, /* 010 */
    0, /* 011 */
    1, /* 100 */
    0, /* 101 */
    1, /* 110 */
    1  /* 111 */
};

seq_t a(idx_t n)
{
    size_t width = (size_t)(2 * n + 1);          /* row length in bits */
    uint8_t *curr = (uint8_t *)malloc(width);   /* current row */
    uint8_t *next = (uint8_t *)malloc(width);   /* next row */

    if (!curr || !next)                        /* out of memory */
        return 0;

    /* initialise: all zeros except the centre cell */
    for (size_t i = 0; i < width; ++i) curr[i] = 0;
    curr[n] = 1;

    /* iterate the automaton */
    for (idx_t step = 1; step <= n; ++step)
    {
        for (size_t i = 0; i < width; ++i)
        {
            uint8_t left   = (i == 0          ? 0 : curr[i - 1]);
            uint8_t center = curr[i];
            uint8_t right  = (i + 1 == width ? 0 : curr[i + 1]);

            uint8_t pattern = (left << 2) | (center << 1) | right;
            next[i] = rule_table[pattern];
        }

        /* swap pointers for next iteration */
        uint8_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* convert the final binary row to decimal in seq_t */
    seq_t value = 0;
    for (size_t i = 0; i < width; ++i)
    {
        value = value * 2 + (seq_t)curr[i];
    }

    /* cleanup */
    free(curr);
    free(next);
    return value;
}
