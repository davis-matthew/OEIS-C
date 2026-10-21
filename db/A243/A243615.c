/*
 * A243615  Numbers n whose digital sum equals the number of binary digits in
 *          its binary expansion.
 * https://oeis.org/A243615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 2918, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth positive integer whose decimal digit sum equals its
   binary bit length (i.e. digit_sum(m) == bitlen_seq_t(m)). */

#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Count how many qualifying numbers have been found so far. */
    idx_t count = 0;

    /* Current candidate number.  Starts at 1 because a(1) = 1. */
    seq_t cand = 1;

    for (;; cand++) {
        /* Condition of the sequence. */
        if (digit_sum(cand) == bitlen_seq_t(cand)) {
            count++;
            if (count == n) {
                return cand;
            }
        }
    }
}
