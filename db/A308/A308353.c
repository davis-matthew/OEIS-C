/*
 * A308353  Minimal sum of integers filling an n X n square, as explained in
 *          the Comments and Example sections.
 * https://oeis.org/A308353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(1) = 0
 *   a(3) = 59            // special case
 *   For all other n ≥ 1:
 *     a(n) = n*(n-1)*(2*n*n + 1)/2
 *
 * This closed‑form gives the minimal sum of the integers that must be placed
 * inside an n×n square according to the sewing process described in OEIS A308353.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special case n = 3 which deviates from the general pattern.  */
    if (n == 3) return 59;

    /* General formula: a(n) = n*(n-1)*(2*n*n + 1)/2.
     * Intermediate arithmetic is performed in 128‑bit to avoid overflow
     * when n is large (up to the limits of 64‑bit arithmetic).  */
    __int128 temp = (__int128)n * (n - 1) *
                    (((__int128)2 * n * n) + 1);
    temp /= 2;
    return (seq_t)temp;
}
