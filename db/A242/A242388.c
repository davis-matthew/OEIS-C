/*
 * A242388  Triangle read by rows: T(n,k) = n*2^(k-1) + 1, 1 <= k <= n.
 * https://oeis.org/A242388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r*2^(k-1) + 1 where r = smallest integer with r*(r+1)/2
 *            >= n, and k = n - (r-1)*r/2. The sequence is obtained by reading
 *            the triangle T(n,k) = n*2^(k-1)+1 row‑by‑row
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row index r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }
    /* Compute k (position within the row) */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;
    /* Compute the term: r * 2^(k-1) + 1 */
    seq_t term = ((seq_t)r) * pow2(k - 1) + 1;
    return term;
}
