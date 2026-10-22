/*
 * A358125  Triangle read by rows: T(n, k) = 2^n - 2^(n-k-1) - 2^k, 0 <= k <=
 *          n-1.
 * https://oeis.org/A358125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r = smallest integer with r(r+1)/2 >= n (row
 *            number) k = n - r(r-1)/2 - 1 (column index, 0‑based) T(r,k) =
 *            2^r - 2^(r-k-1) - 2^k
 * Range:     u8 n <= 37, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* find the row r */
    idx_t r = 1;
    while ( (r * (r + 1)) / 2 < n )
        ++r;

    /* previous triangular number (elements before this row) */
    idx_t prev = (r * (r - 1)) / 2;

    /* column index k (0‑based) */
    idx_t k = n - prev - 1;

    /* compute powers of two */
    seq_t pow_r   = pow2(r);
    seq_t pow_rk1 = pow2(r - k - 1);
    seq_t pow_k   = pow2(k);

    /* T(r,k) = 2^r - 2^(r-k-1) - 2^k */
    return pow_r - pow_rk1 - pow_k;
}
