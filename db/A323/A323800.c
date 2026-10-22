/*
 * A323800  Number of permutations p of [n] such that max_{j=1..n} |p(j)-j| =
 *          4.
 * https://oeis.org/A323800
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = count_perm(n,4) – count_perm(n,3) where count_perm(n,k)
 *            counts permutations of [n] with max_{j} |p(j)-j| ≤ k
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   count_perm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The DP keeps a sliding window of size 2k+1 and uses a bitmask
   to describe which positions in the current window are already
   occupied.  The leftmost position that falls out of the window
   must be matched if it is still free.  */

#include <stdlib.h>
#include <string.h>
#include "../utils/count_perm.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    seq_t up4 = count_perm(n, 4);
    seq_t up3 = count_perm(n, 3);
    return up4 - up3;
}
