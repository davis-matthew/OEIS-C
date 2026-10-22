/*
 * A356841  Numbers k such that the k-th composition in standard order covers
 *          an interval of positive integers (gapless).
 * https://oeis.org/A356841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   is_gapless
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th non‑negative integer k such that the
   k-th composition in standard order (graded reverse‑lexicographic)
   is gapless, i.e. the multiset of its part sizes forms a set of
   consecutive integers. The algorithm enumerates k, extracts the
   composition from the binary representation of k and tests the
   gapless property. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_gapless.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;

    for (;;) {
        if (is_gapless(k)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
