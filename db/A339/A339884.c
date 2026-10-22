/*
 * A339884  Triangle read by rows: T(n, m) gives the number of partitions of n
 *          with m parts and parts from {1, 2, 3}.
 * https://oeis.org/A339884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = number of partitions of r with exactly m parts from
 *            {1,2,3}, where (r,m) corresponds to the nth element of the
 *            triangle read by rows. The count equals the number of integer w3
 *            in max(0, r-2m) ≤ w3 ≤ floor((r-m)/2); the result is that range
 *            size
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   triangular_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/triangular_idx.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    /* Find row r such that r*(r+1)/2 >= n */
    idx_t low = 1, high = 1;
    __int128 tmp;
    /* Exponentially increase high until enough */
    while ((__int128)high * (high + 1) / 2 < n) high <<= 1;
    /* Binary search */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri_mid = (__int128)mid * (mid + 1) / 2;
        if (tri_mid >= n) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    idx_t r = low;                     /* row number */
    /* Previous cumulative count */
    idx_t prev = (idx_t)((__int128)(r - 1) * r / 2);
    idx_t m = n - prev;                 /* column 1‑based */

    /* Compute bounds for w3 */
    idx_t L = (r > 2 * m) ? (r - 2 * m) : 0;          /* max(0, r-2m) */
    idx_t U = (r - m) / 2;                            /* floor((r-m)/2) */

    if (L > U) return 0;
    seq_t count = (seq_t)(U - L + 1);
    return count;
}
