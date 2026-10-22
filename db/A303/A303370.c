/*
 * A303370  Least integer k such that (k+1)^k >= n.
 * https://oeis.org/A303370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k >= 0 such that (k+1)^k >= n The sequence is
 *            monotone, so a binary search on k works
 * Range:     u8 n <= 2111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   power_geq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/power_geq.h"

seq_t a(idx_t n) {
    /* Convert the index to seq_t for comparisons. */
    seq_t n_val = (seq_t)n;

    /* Binary search for the minimal k. */
    seq_t low = 0;
    seq_t high = n;          /* (k+1)^k is strictly increasing for k >= 0 */
    while (low < high) {
        seq_t mid = low + (high - low) / 2;
        if (power_geq(mid, n_val))
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}
