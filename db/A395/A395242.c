/*
 * A395242  Numbers whose binary representation contains isolated digits "11"
 *          at least once.
 * https://oeis.org/A395242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose binary representation
 *            contains at least one isolated run of exactly two consecutive 1
 *            bits
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_isolated_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_isolated_11.h"

seq_t a(idx_t n)
{
    /* Scan forward until we have found the n‑th term. */
    seq_t cand = 1;
    idx_t count = 0;

    while (1) {
        if (has_isolated_11(cand)) {
            ++count;
            if (count == n)
                return cand;
        }
        ++cand;
    }
}
