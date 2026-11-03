/*
 * A377169  Nonnegative integers containing isolated ones in their binary
 *          representation.
 * https://oeis.org/A377169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9929 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:
   a(n) is the n‑th positive integer whose binary representation
   contains at least one isolated 1 (a 1 that has 0s on both sides if
   such neighbors exist).  a(1) = 1, a(2) = 2, a(3) = 4, … */

#include <stdbool.h>

/* Return true if x has at least one isolated 1 in its binary representation. */
static bool has_isolated_one(seq_t x)
{
    /* isolate 1s that are not adjacent to other 1s on either side */
    return ((x & (~(x << 1)) & (~(x >> 1))) != 0);
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (seq_t cand = 1; ; ++cand) {
        if (has_isolated_one(cand)) {
            ++count;
            if (count == n) {
                return cand;
            }
        }
    }
}
