/*
 * A343311  Numbers of the form x + y + z with distinct positive integers
 *          x,y,z such that (x+y+z) | x*y*z.
 * https://oeis.org/A343311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_valid_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A343311: numbers n = x + y + z with distinct positive integers x, y, z
    such that n | x*y*z.
    The function searches for each requested index n by brute force:
    it enumerates composite candidates in increasing order and tests all
    triples (x, y, z) with x < y < z, x + y + z = candidate.
    The first n such numbers are returned.  This implements the OEIS
    definition without any hard‑coded table.                           */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_valid_triple.h"

seq_t a(idx_t n)
{
    /* Count how many valid numbers have been found so far. */
    idx_t count = 0;

    /* Candidate numbers start at 6 (the first term of the sequence). */
    for (idx_t candidate = 6;; ++candidate) {
        if (!has_valid_triple(candidate))
            continue;            /* skip numbers that are not in the sequence */

        ++count;
        if (count == n)
            return (seq_t)candidate;
    }
}
