/*
 * A337994  T(n, k) =
 *          (k*(2*k+2)*(2*k+1)*(2*n-1)*binomial(2*(n-1),n-1))/(n*(n+1)*(n+2))
 *          for n, k > 0 and T(0, 0) = 1. Triangle read by rows, for 0 <= k <=
 *          n.
 * https://oeis.org/A337994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n, k) = (k*(2*k+2)*(2*k+1)*(2*n-1)*binomial(2*(n-1),n-1))
 *            /(n*(n+1)*(n+2)) for n, k > 0 T(0, 0) = 1. The OEIS sequence
 *            A337994 is the triangle read by rows (0‑based); a(n) is the n‑th
 *            entry in this row‑major traversal. This function computes a(n)
 *            directly
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static inline idx_t find_row(idx_t n)
{
    /* Find the largest r such that r*(r+1)/2 <= n.
       Uses binary search on the 64‑bit index range. */
    idx_t low = 0, high = n;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        seq_t val = (seq_t)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* row index */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* T(0,0) */

    idx_t row = find_row(n);            /* r */
    /* prefix = number of entries in rows 0..row-1 */
    seq_t prefix_s = (seq_t)row * (row + 1) / 2;
    idx_t prefix = (idx_t)prefix_s;
    idx_t k = n - prefix;               /* column index */

    if (k == 0)
        return (seq_t)0;               /* T(r,0) for r>0 */

    seq_t k_s = (seq_t)k;
    seq_t r_s = (seq_t)row;

    /* binomial(2*(r-1), r-1)  */
    seq_t binom = binomial(2 * (row - 1), row - 1);

    seq_t part1 = 2 * k_s + 2;          /* 2k+2 */
    seq_t part2 = 2 * k_s + 1;          /* 2k+1 */
    seq_t part3 = 2 * r_s - 1;          /* 2r-1 */

    seq_t numer = k_s * part1 * part2 * part3 * binom;
    seq_t denom = r_s * (r_s + 1) * (r_s + 2);

    return numer / denom;
}
