/*
 * A339276  Nearest integer to the fourth root of n.
 * https://oeis.org/A339276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to n^(1/4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_fourth_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_fourth_root.h"

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
