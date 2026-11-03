/*
 * A370258  Triangle read by rows: T(n, k) = binomial(n, k)*binomial(2*n+k,
 *          k), 0 <= k <= n.
 * https://oeis.org/A370258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = flatten row‑major enumeration of the triangle T(n,k) =
 *            C(n,k) * C(2*n + k, k), 0 ≤ k ≤ n. The mapping (r,k) is obtained
 *            by unflattening the triangle using the triangular numbers
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 85, u64 n <= 290,
 *            u128 n <= 977, big any n
 * Validated: standard, pass, 68 terms through n = 979 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t low = 0;
    idx_t high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Offset within row r */
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start;

    /* Compute the triangle entry T(r,k) */
    seq_t left  = binomial(r, k);
    seq_t right = binomial(2 * r + k, k);
    return left * right;
}
