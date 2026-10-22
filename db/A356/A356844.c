/*
 * A356844  Numbers k such that the k-th composition in standard order
 *          contains at least one 1. Numbers that are odd or whose binary
 *          expansion contains at least two adjacent 1's.
 * https://oeis.org/A356844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that k is odd or its binary
 *            representation contains at least two adjacent 1's
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   has_adjacent_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_adjacent_ones.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for offset 1, return 0 */

    idx_t count = 0;
    idx_t x = 1;

    for (;; ++x) {
        if ((x & 1) || has_adjacent_ones(x)) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
