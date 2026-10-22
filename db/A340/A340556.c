/*
 * A340556  E2(n, k), the second-order Eulerian numbers with E2(0, k) = δ_{0,
 *          k}. Triangle read by rows, E2(n, k) for 0 <= k <= n.
 * https://oeis.org/A340556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = E2(r, k) where r = the smallest non‑negative integer with
 *            (r+1)(r+2)/2 > n, k = n - r(r+1)/2. The second‑order Eulerian
 *            numbers satisfy E2(0,0) = 1, E2(n,k) = E2(n-1,k)*k +
 *            E2(n-1,k-1)*(2*n - k)
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   compute_E2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compute_E2.h"

/* Determine the row index r for a given linear index n.  The
   bounding inequality is (r+1)(r+2)/2 > n.  The loop is tiny
   because r is on the order of sqrt(n). */
static idx_t row_for_idx(idx_t n)
{
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = row_for_idx(n);
    /* total entries up to row r-1 is r*(r+1)/2 */
    idx_t prev_total = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - prev_total;
    return compute_E2(r, k);
}
