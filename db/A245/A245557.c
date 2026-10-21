/*
 * A245557  Irregular triangle read by rows: T(n,k) (n>=0, 0 <= k <= 2n) =
 *          number of triples (u,v,w) with entries in the range 0 to n which
 *          have some pair adding up to k and in which at least one of u,v,w
 *          is equal to n.
 * https://oeis.org/A245557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) * where r = ⌊√n⌋, k = n - r², * and T(r,k) =
 *            3*(k+1) for 0 ≤ k ≤ r-1 * T(r,k) = 12*r - 3*k - 3 for r ≤ k ≤
 *            2r-1 * T(r,k) = 3*r + 1 for k = 2r
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned 64‑bit integer.
   Returns ⌊√n⌋.  Uses binary search with 128‑bit arithmetic for safety. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = ((idx_t)1 << 32) - 1;          /* ≥ sqrt(UINT64_MAX) */
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t r     = isqrt_u64(n);          /* row number */
    idx_t r_sq  = r * r;                 /* r² fits in 64 bits */
    idx_t k     = n - r_sq;              /* position inside the row */

    seq_t r_t = (seq_t)r;
    seq_t k_t = (seq_t)k;

    if (k < r)                 /* first segment */
        return 3 * (k_t + 1);
    else if (k < 2 * r)        /* middle segment */
        return 12 * r_t - 3 * k_t - 3;
    else                       /* last element of the row */
        return 3 * r_t + 1;
}
