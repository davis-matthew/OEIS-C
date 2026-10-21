/*
 * A297126  Numbers whose base-3 digits d(m), d(m-1),..., d(0) have m=0 or
 *          else d(i) = d(i+1) for some i in {0,1,...,m-1}.
 * https://oeis.org/A297126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_adjacent_equal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297126:
   a(n) = nth positive integer whose base‑3 representation has
   at least one pair of adjacent equal digits. */

/* -------------------------------------------------------------
 * Helper: check if a number has an adjacent pair of equal digits
 * in base 3. For numbers 1 and 2 (one‑digit numbers) the
 * condition is automatically satisfied.
 * ------------------------------------------------------------- */
#include <stdbool.h>
#include "../utils/has_adjacent_equal.h"

/* -------------------------------------------------------------
 * Main function required by the build system.
 * ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t current = 1;            /* start from 1 */

    while (true) {
        if (has_adjacent_equal(current)) {
            ++found;
            if (found == n)
                return current;
        }
        ++current;
    }
}
