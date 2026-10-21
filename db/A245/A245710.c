/*
 * A245710  Number of nonzero evil numbers <= n, see A001969.
 * https://oeis.org/A245710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # {k : 1 <= k <= n, popcount(k) is even}
 * Range:     u8 n <= 512, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   even_up_to, msb_position
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/even_up_to.h"
#include "../utils/msb_position.h"
/* Memoization not required; a recursive bit‑DP gives the answer in O(log n) time. */

seq_t a(idx_t n)
{
    seq_t ev = even_up_to(n);  /* includes 0 */
    return ev - (seq_t)1;      /* exclude the non‑zero restriction */
}
