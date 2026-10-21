/*
 * A265282  Number of triangles in a certain geometric structure: see
 *          "Illustration of initial terms" link for precise definition.
 * https://oeis.org/A265282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 15*n^2 + 57*n - 8 + (3*n^2 - n + 4)*(-1)^n * +
 *            4*(-1)^((2*n-1+(-1)^n)/4))/64
 * Range:     u8 n <= 17, u16 n <= 125, u32 n <= 5157, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use signed 128‑bit for intermediate values to avoid overflow and
     * to handle negative terms safely.                                            */
    __int128 nn = (__int128)n;

    /* (-1)^n  ->  +1 for even n, -1 for odd n                         */
    int64_t sgn1 = ((nn & 1) == 0) ? 1 : -1;
    /* (-1)^floor(n/2)  ->  +1 for even floor(n/2), -1 for odd floor(n/2) */
    int64_t sgn2 = (((nn >> 1) & 1) == 0) ? 1 : -1;

    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;

    /* numerator of the fraction;  all operations are exact  */
    __int128 num = 2 * n3 + 15 * n2 + 57 * nn - 8
                   + (3 * n2 - nn + 4) * sgn1
                   + 4 * sgn2;

    /* the division by 64 is exact for all n */
    uint64_t res64 = (uint64_t)(num / 64);
    return (seq_t)res64;
}
