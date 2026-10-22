/*
 * A374767  Numbers k such that the leaders of strictly decreasing runs in the
 *          k-th composition in standard order are distinct.
 * https://oeis.org/A374767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   distinct_leaders_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A374767:
   a(n) is the n‑th nonnegative integer k such that the leaders
   of the strictly decreasing runs in the k‑th composition
   (in standard order) are pairwise distinct.  This is done by
   generating the composition for each k and checking the leaders
   linearly.  The algorithm runs in O(n·log k) time for k up to 2^64. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/distinct_leaders_of.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (uint64_t k = 0; ; ++k) {
        if (distinct_leaders_of(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
