/*
 * A360033  Table T(n,k), n >= 1 and k >= 0, read by antidiagonals, related to
 *          Jacobsthal numbers A001045.
 * https://oeis.org/A360033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   jacobsthal_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A360033
 *
 *  Definition: let T(n,k) = 2^k * n - J(k) where J(k) is the Jacobsthal
 *  number (J(0)=0, J(1)=1, J(k)=J(k-1)+2*J(k-2)).  Then a(n) is the
 *  sequence obtained by reading the table T(n,k) (n ≥ 1, k ≥ 0) along
 *  antidiagonals, with each anti‑diagonal listed from largest n to
 *  smallest n.
 *
 *  Computation:
 *      1. Determine the anti‑diagonal d that contains the n-th element.
 *         d is the largest integer such that d*(d+1)/2 < n.
 *      2. Compute the offset within that diagonal:
 *         offset = n - d*(d+1)/2 - 1   (0‑based)
 *      3. The table entry is
 *         k = offset
 *         n' = d + 1 - offset           (row index in the table)
 *      4. Compute T(n',k) using the recurrence
 *         T(n',0) = n'
 *         T(n',i) = 2*T(n',i-1) + (-1)^i   for i = 1 .. k
 *
 *  This algorithm runs in O(k) time per call (k ≤ d ≤ O(√n)).
 */

#include <stdint.h>
#include "../utils/jacobsthal_at.h"

/* --------------------------------------------------------------------- */
static seq_t pow2_idx(idx_t exp)
{
    /* Compute 2^exp as seq_t; works for 0 ≤ exp < bitlen(seq_t). */
    seq_t one = (seq_t)1;
    return one << exp;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* undefined for offset 0; return 0 */

    /* Find the anti‑diagonal d such that d*(d+1)/2 < n <= (d+1)(d+2)/2. */
    idx_t low  = 0;
    idx_t high = n;          /* safe upper bound for search */
    idx_t d    = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri < n) {
            d = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Compute offset within diagonal d, then n' and k. */
    unsigned __int128 prev = (unsigned __int128)d * (d + 1) / 2;
    idx_t offset = n - (idx_t)prev - 1;   /* 0‑based offset in diagonal */
    idx_t n_table = d + 1 - offset;       /* row index in the table */
    idx_t k = offset;                     /* column index in the table */

    /* Evaluate T(n_table, k) by recurrence. */
    seq_t val = (seq_t)n_table;
    for (idx_t i = 1; i <= k; ++i) {
        val <<= 1;                 /* val = 2*val */
        if (i & 1)                 /* odd i: subtract 1 */
            val -= (seq_t)1;
        else                       /* even i: add 1 */
            val += (seq_t)1;
    }
    return val;
}
