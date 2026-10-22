/*
 * A375085  Triangle read by rows: T(n,k) is the number of ballotlike paths
 *          ending at (n, k), with 0 <= k <= n.
 * https://oeis.org/A375085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by flattening the triangle T(n,k) defined for 0
 *            ≤ k ≤ n: T(n,k) = binomial(2n-2, n-k-1) - binomial(2n-2, n-k-2)
 *            + binomial(n-2, n-k) with the special cases T(0,0)=0, T(1,0)=0,
 *            T(1,1)=1. The sequence is read row‑by‑row: first the row 0
 *            entry, then the entries of row 1, then of row...
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /*--- Special case for the very first element ---*/
    if (n == 0)
        return 0;

    /*--- Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 ---*/
    idx_t m = 8 * n + 1;
    idx_t s = _floor_isqrt_u64(m);          /* floor(sqrt(8n+1)) */
    idx_t r = (s - 1) >> 1;                 /* r = (s-1)/2 */
    idx_t prev = r * (r + 1) / 2;           /* number of elements before row r */
    idx_t k   = n - prev;                   /* column within the row */

    /*--- Handle the tiny rows separately (to avoid negative binomial arguments) ---*/
    if (r == 0)
        return 0;
    if (r == 1) {
        if (k == 0)
            return 0;          /* T(1,0) */
        if (k == 1)
            return 1;          /* T(1,1) */
        return 0;              /* impossible but keeps compiler happy */
    }

    /*--- For r ≥ 2 use the general formula ---*/
    seq_t a1 = 0, a2 = 0, a3 = 0;

    /* term1: binomial(2r-2, r-k-1) */
    if (k <= r - 1)
        a1 = binomial(2 * r - 2, r - k - 1);

    /* term2: binomial(2r-2, r-k-2) */
    if (k <= r - 2)
        a2 = binomial(2 * r - 2, r - k - 2);

    /* term3: binomial(r-2, r-k) */
    idx_t rk = r - k;
    if (rk <= r - 2)
        a3 = binomial(r - 2, rk);

    /* Since a1 ≥ a2, subtraction is safe */
    seq_t res = a1 - a2;
    res += a3;
    return res;
}
