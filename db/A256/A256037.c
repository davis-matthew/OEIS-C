/*
 * A256037  Triangle read by rows: number of R-class idempotents of rank k in
 *          Brauer monoid B_n.
 * https://oeis.org/A256037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained from the triangle * T(r,k) = (r + k - 1)!! /
 *            (2*k - 1)!! if (r + k) is even, * T(r,k) = 0 otherwise, * where
 *            r ≥ k ≥ 0 and a(0) = 1. * The linear index n is converted to
 *            (r,k) by triangular numbering
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t low  = 0;
    idx_t high = n;              /* r can never exceed n */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    idx_t r = high;
    idx_t tri_r = r * (r + 1) / 2;
    idx_t k = n - tri_r;         /* column index */

    /* Parity check: result is zero if r + k is odd */
    if ( ((r + k) & 1U) == 1U )
        return 0;

    /* Use signed arithmetic for possible negative intermediate values.
       r and k are small (≤ sqrt(2*n)), so int64_t is safe. */
    int64_t r_int = (int64_t) r;
    int64_t k_int = (int64_t) k;
    int64_t start = 2 * k_int + 1;        /* first odd in product */
    int64_t end   = r_int + k_int - 1;    /* last odd in product */

    seq_t result = 1;
    for (int64_t x = start; x <= end; x += 2) {
        result *= (seq_t) x;             /* product of odd numbers */
    }
    return result;
}
