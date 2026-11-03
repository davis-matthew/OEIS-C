/*
 * A245710  Number of nonzero evil numbers <= n, see A001969.
 * https://oeis.org/A245710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # {k : 1 <= k <= n, popcount(k) is even}
 * Range:     u8 n <= 512, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9473 (2026-11-01)
 * Helpers:   ilog2_idx_to_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ilog2_idx_to_i32.h"

/* Count numbers in [0, n] whose binary popcount is even. */
static seq_t even_up_to(idx_t n)
{
    if (n == 0)                 /* only 0 itself, which is evil */
        return (seq_t)1;

    int msb = ilog2_idx_to_i32(n); /* position of highest set bit */
    idx_t pow2 = ((idx_t)1) << msb;

    /* Numbers < pow2: exactly half are evil.  For msb==0 only 0 is in this range. */
    seq_t ev_even = (msb == 0) ? (seq_t)1 : ((seq_t)1 << (msb - 1));

    /* Remaining numbers: pow2 + 1 ... n.
       Their leading bit is 1, so we need the odd‑popcount part of the rest. */
    idx_t rem = n - pow2;
    seq_t odd_in_rem = (seq_t)(rem + 1) - even_up_to(rem);

    return ev_even + odd_in_rem;
}
/* Memoization not required; a recursive bit‑DP gives the answer in O(log n) time. */

seq_t a(idx_t n)
{
    seq_t ev = even_up_to(n);  /* includes 0 */
    return ev - (seq_t)1;      /* exclude the non‑zero restriction */
}
