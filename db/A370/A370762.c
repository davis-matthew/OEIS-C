/*
 * A370762  Triangle read by rows: T(n,k) = 2 * (k mod 2 + 1) *
 *          T(n-1,floor(k/2)) + 1 with T(0,0) = 1 for 0 <= k <= 2^n-1.
 * https://oeis.org/A370762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Triangle T(n,k) = 2*(k mod 2 + 1)*T(n-1, floor(k/2)) + 1, with
 *            T(0,0) = 1. The sequence a(n) is the flattened triangle read
 *            row‑wise starting at n = 0. This implementation computes a(n) by
 *            determining the row and column of the flattened index and then
 *            evaluating T(n,k) iteratively from the most s...
 * Range:     u8 n <= 29, u16 n <= 509, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(log2(x)) for a positive integer x (x >= 1).  */
static inline idx_t floor_log2_idx(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* If n == 0, r = 0 and k = 0; value = 1. */
    idx_t r = floor_log2_idx(n + 1);          /* row number */
    idx_t offset = ((idx_t)1 << r) - 1;        /* total elements before this row */
    idx_t k = n - offset;                      /* column index within the row */

    seq_t v = 1;  /* base value T(0,0) */

    for (idx_t i = r; i > 0; --i) {
        idx_t bit = (k >> (i - 1)) & 1;         /* next most significant bit of k */
        v = 2 * ((seq_t)bit + 1) * v + 1;       /* T(current) = 2*(bit+1)*T(parent)+1 */
    }

    return v;
}
