/*
 * A268412  Balanced evil numbers: numbers with an even number of runs of 1's
 *          in their binary expansion.
 * https://oeis.org/A268412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 8256, u32 n <= 8256, u64 n <= 8256,
 *            u128 n <= 8256, big any n
 * Helpers:   even_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268412: Balanced evil numbers.
   Balanced evil numbers are integers whose binary representation
   contains an even number of runs of 1's.  The function returns
   the n‑th such number (a(0) = 0, a(1) = 5, …).  It searches
   integers in increasing order until the n‑th qualifying number
   is found. */

#include <stdbool.h>
#include "../utils/even_runs.h"

/* The actual sequence function.  idx_t is unsigned integer type. */
seq_t a(idx_t n)
{
    seq_t x = 0;
    idx_t count = 0;
    for (;;) {
        if (even_runs(x)) {
            if (count == n)
                return x;
            ++count;
        }
        ++x;
    }
}
