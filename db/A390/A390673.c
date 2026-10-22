/*
 * A390673  Numbers k such that the k-th composition in standard order has all
 *          distinct first sums.
 * https://oeis.org/A390673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 4973, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   hasDistinctFirstSums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k such that the k-th standard composition
   has all distinct first sums. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/hasDistinctFirstSums.h"

/* Main function required by the problem statement. */
seq_t a(idx_t n)
{
    idx_t k = 0;
    idx_t found = 0;

    while (1) {
        if (hasDistinctFirstSums(k)) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        ++k;
    }
}
