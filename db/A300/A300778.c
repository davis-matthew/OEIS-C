/*
 * A300778  Number of grid points visible from a corner of an m X n
 *          rectangular region on a square grid written as triangle T(m,n), 1
 *          <= n <= m.
 * https://oeis.org/A300778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2 + |{ (x,y) : 1 ≤ x ≤ m, 1 ≤ y ≤ k, gcd(x,y)=1 }| where
 *            (m,k) is obtained from the triangular indexing: the nth term
 *            lies in row m (1 ≤ k ≤ m) of the triangle
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   simple_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any seq_t and idx_t types and is
   O(m·k) per call, with m ≈ √(2·n) ≤ 141 for n ≤ 10000. */

#include <stddef.h>
#include "../utils/simple_gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, but guard is harmless */

    /* Find the row m such that n <= m(m+1)/2 */
    idx_t m = 0;
    while (m * (m + 1) / 2 < n) ++m;

    /* Number of terms before this row */
    idx_t prev = (m - 1) * m / 2;

    /* Column index k (1-based) within the row */
    idx_t k = n - prev;

    /* Count coprime pairs (x,y) with 1 ≤ x ≤ m, 1 ≤ y ≤ k */
    seq_t cnt = 0;
    for (idx_t x = 1; x <= m; ++x) {
        for (idx_t y = 1; y <= k; ++y) {
            if (simple_gcd(x, y) == 1) ++cnt;
        }
    }

    /* Add the constant 2 as defined by the sequence */
    return cnt + 2;
}
