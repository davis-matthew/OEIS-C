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
 * Validated: standard, pass, 106 terms through n = 8635 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool power_geq(seq_t k, seq_t n_val) {
    /* Return true iff (k+1)^k >= n_val.
       The loop stops as soon as the intermediate power would exceed n_val
       to avoid overflow on seq_t of bounded size. */
    seq_t base = k + 1;
    seq_t power = 1;
    for (idx_t i = 0; i < (idx_t)k; ++i) {
        /* If multiplying would overflow beyond n_val, we can stop. */
        if (power > n_val / base)
            return true;
        power *= base;
    }
    return power >= n_val;
}

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
