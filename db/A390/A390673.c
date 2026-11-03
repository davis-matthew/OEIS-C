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
 * Validated: standard, pass, 81 terms through n = 8032 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k such that the k-th standard composition
   has all distinct first sums. */
#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether the k‑th standard composition has all
   distinct first sums. */
static bool hasDistinctFirstSums(idx_t k)
{
    /* differences between successive 1‑bit positions, with an
       implicit leading 0. */
    idx_t diffs[128];
    int ndiff = 0;
    idx_t prev = 0;
    idx_t bits = k;
    idx_t bitindex = 0;

    while (bits > 0) {
        if (bits & 1) {
            idx_t pos = bitindex + 1;   /* 1‑based position */
            diffs[ndiff++] = pos - prev;
            prev = pos;
        }
        bitindex++;
        bits >>= 1;
    }

    /* A composition of length 0 or 1 has no first sums, so it
       automatically satisfies the property. */
    if (ndiff <= 1)
        return true;

    /* All first sums must be distinct.  The sum of two differences
       is at most 2 * (bitindex + 1), which is <= 256 for 64‑bit
       integers; a small bool array is enough. */
    bool seen[512] = {0};   /* enough capacity for all possible sums */
    for (int i = 0; i < ndiff - 1; ++i) {
        unsigned int sum = diffs[i] + diffs[i + 1];
        if (seen[sum])
            return false;
        seen[sum] = true;
    }
    return true;
}

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
