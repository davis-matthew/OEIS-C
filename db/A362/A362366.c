/*
 * A362366  Square array A(n, k), n, k >= 0, read by antidiagonals; A(n, k) is
 *          the least base >= 2 where the sum n + k can be computed without
 *          carry.
 * https://oeis.org/A362366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9958, u16 n <= 9958, u32 n <= 9958, u64 n <= 9958,
 *            u128 n <= 9958, big any n
 * Helpers:   base_for_pair, diag_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least base b ≥ 2 for which addition in base b of the pair (i,j) on
   the antidiagonal of index n has no carry.  a(n) returns that base. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/base_for_pair.h"
#include "../utils/diag_sum.h"

seq_t a(idx_t n)
{
    /* Locate the coordinates (i,j) corresponding to index n. */
    idx_t s   = diag_sum(n);
    idx_t tri = (idx_t)((unsigned __int128)s * (s + 1) / 2);
    idx_t pos = n - tri;          /* position along the diagonal   */
    idx_t i   = pos;
    idx_t j   = s - pos;

    return base_for_pair(i, j, s);
}
