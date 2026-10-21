/*
 * A267139  Decimal representation of the n-th iteration of the "Rule 103"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, n<11: use given initial values. For n > 10: a(n) =
 *            19*a(n-2) - 50*a(n-4) + 32*a(n-6). The sequence A267139 is the
 *            decimal representation of the n-th iteration of Rule 103
 *            cellular automaton starting with a single ON cell
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[11] = {
    1ULL,    /* a(0) */
    6ULL,    /* a(1) */
    6ULL,    /* a(2) */
    117ULL,  /* a(3) */
    60ULL,   /* a(4) */
    1931ULL, /* a(5) */
    376ULL,  /* a(6) */
    32535ULL,/* a(7) */
    752ULL,  /* a(8) */
    523823ULL,/* a(9) */
    1504ULL  /* a(10) */
};

seq_t a(idx_t n)
{
    if (n < 11)
        return init[n];

    /* Allocate an array to hold all terms up to n. */
    seq_t *buf = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!buf) abort();            /* simple error handling */

    /* Initialise with the known first 11 terms. */
    for (idx_t i = 0; i < 11; ++i)
        buf[i] = init[i];

    /* Compute remaining terms using the linear recurrence. */
    for (idx_t i = 11; i <= n; ++i) {
        /* The constants are small, so unsigned arithmetic is safe
           for all n that fit into seq_t; for larger values a
           seq_t that is an arbitrary‑precision integer handles
           the large intermediate values. */
        seq_t term = buf[i - 2] * 19 + buf[i - 6] * 32 - buf[i - 4] * 50;
        buf[i] = term;
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
