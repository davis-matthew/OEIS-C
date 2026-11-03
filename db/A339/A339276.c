/*
 * A339276  Nearest integer to the fourth root of n.
 * https://oeis.org/A339276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to n^(1/4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9613 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer floor of the fourth root of n (n <= 2^64-1).
   Uses a binary search on the interval [0,65535], which contains
   all possible fourth roots of 64‑bit integers. */
static idx_t floor_fourth_root(idx_t n)
{
    idx_t low = 0;
    idx_t high = 65535;               /* maximum possible fourth root */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        idx_t mid2 = mid * mid;       /* mid^2 <= 2^32-1 */
        idx_t mid4 = mid2 * mid2;     /* mid^4 < 2^64 */

        if (mid4 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = floor_fourth_root(n);

    /* Determine whether n^(1/4) is closer to r or r+1.
       The threshold occurs at (r + 0.5)^4 = (2r+1)^4 / 16. */
    idx_t mid = 2 * r + 1;
    unsigned __int128 mid4 = (unsigned __int128)mid * mid * mid * mid;
    unsigned __int128 lhs  = (unsigned __int128)16 * n;

    return (lhs <= mid4) ? (seq_t)r : (seq_t)(r + 1);
}
