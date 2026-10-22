/*
 * A359794  Union of odd numbers and numbers with an odd 2-adic valuation.
 * https://oeis.org/A359794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer whose 2‑adic valuation is not
 *            even and at least 2 (i.e., numbers that are NOT of the form
 *            (odd)·4^k, k ≥ 1). This is implemented by a binary search over
 *            the rank function rank(x) = x – #{n ≤ x : v₂(n) is an even
 *            integer ≥ 2}
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   count_excluded
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_excluded.h"

/* Rank of x: number of included integers ≤ x. */
static idx_t rank_of(idx_t x)
{
    return x - count_excluded(x);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, guard */

    /* Find an upper bound high such that rank_of(high) ≥ n. */
    idx_t low = 1;
    idx_t high = n ? n : 1;
    while (rank_of(high) < n) {
        if (high > UINT64_MAX / 2) {        /* stop before overflow */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* Binary search for minimal x with rank(x) ≥ n. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (rank_of(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
