/*
 * A277009  Numbers not in range of A277012: numbers such that at least one
 *          run of 1-bits in their binary expansion is longer than 1 + the
 *          total number of 0-bits anywhere right of that run.
 * https://oeis.org/A277009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A277009: numbers whose binary expansion contains a run
   of 1‑bits whose length is greater than 1 plus the number of
   zero‑bits anywhere to its right. The function scans numbers
   sequentially until the requested index is reached. */

#include <stdbool.h>
#include <stdint.h>

/* Test whether a number satisfies the A277009 condition. */
static bool has_property(seq_t x)
{
    seq_t zerosRight = 0;          /* zeros to the right of the current run */
    while (x != 0) {
        if (x & 1) {               /* start of a run of 1‑bits */
            seq_t run_len = 0;
            while (x & 1) {
                ++run_len;
                x >>= 1;
            }
            if (run_len > 1 + zerosRight)
                return true;
            /* zerosRight remains unchanged after a 1‑run */
        } else {                   /* a zero‑bit */
            ++zerosRight;
            x >>= 1;
        }
    }
    return false;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t candidate = 1;
    for (;;) {
        if (has_property(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;   /* candidate = candidate + 1   */
    }
}
