/*
 * A247936  Riordan array ((1-2x)/(1-3x), 2x).
 * https://oeis.org/A247936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * Let r be the unique non‑negative integer satisfying *
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 (row index) * and let c = n -
 *            r*(r+1)/2 (column in that row). * Then * a(n) = 3^(r-c-1) * 2^c
 *            if r > c * a(n) = 2^c if r == c * (The factor 3^(r-c-1) is
 *            omitted when r==c because 3^(-1) is not defined.)
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Find row index r: the largest r with r*(r+1)/2 ≤ n */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2) / 2) <= n )
        ++r;

    /* Column index c within that row */
    idx_t c = n - r * (r + 1) / 2;

    /* m = r - c  is the distance from the right end of the row */
    idx_t m = r - c;

    if (m == 0)                    /* r == c  →  last element of the row */
        return pow2(c);            /* a(n) = 2^c */

    /* General case: a(n) = 3^(m-1) * 2^c   with m = r-c > 0 */
    return pow3(m - 1) * pow2(c);
}
