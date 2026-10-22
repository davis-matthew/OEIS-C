/*
 * A332663  Even bisection of A332662: the x-coordinates of an enumeration of
 *          N X N.
 * https://oeis.org/A332663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = A124171(n+1) - 1
 * Range:     u8 n <= 2279, u16 n <= 9551, u32 n <= 9551, u64 n <= 9551,
 *            u128 n <= 9551, big any n
 * Helpers:   prefix_tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = n - m*(m+1)*(m+2)/6   where
   m is the largest integer with m*(m+1)*(m+2)/6 <= n  */

#include <stdint.h>
#include "../utils/prefix_tri.h"

seq_t a(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Find an upper bound for m */
    while (prefix_tri(high) <= (__uint128_t)n) {
        if (high == UINT64_MAX) {          /* safety for extreme n */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* Binary search for the largest m with prefix_tri(m) <= n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (prefix_tri(mid) <= (__uint128_t)n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t m = low - 1;                      /* correct m */

    idx_t sm = (idx_t)prefix_tri(m);      /* prefix sum S(m) */
    return (seq_t)(n - sm);
}
