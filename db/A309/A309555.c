/*
 * A309555  Triangle read by rows: T(n,k) = 3 + k*(n-k) for n >= 0, 0 <= k <=
 *          n.
 * https://oeis.org/A309555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle read row‑wise, where
 *            the triangle is T(n,k) = 3 + k*(n-k) for 0 ≤ k ≤ n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Determine the row r such that
       r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.                           */
    idx_t r;
    if (n == 0) {
        r = 0;
    } else {
        /* Use a double approximation of the root of 8n+1.       */
        double d = sqrt((double)8.0 * (double)n + 1.0);
        r = (idx_t)((d - 1.0) * 0.5);   /* floor, may be slightly low */

        /* Correct possible inaccuracies from the floating‑point
           approximation.                                            */
        while ( (__uint128_t)r * (r + 1) / 2 > n ) --r;
        while ( (__uint128_t)(r + 1) * (r + 2) / 2 <= n ) ++r;
    }

    /* Compute the offset of the first element of row r.            */
    idx_t offset = (idx_t)((__uint128_t)r * (r + 1) / 2);

    /* Column index k within row r.                                 */
    idx_t k = n - offset;

    /* Value of the triangle entry.                                 */
    seq_t rt = (seq_t)r;
    seq_t kt = (seq_t)k;
    return (seq_t)3 + kt * (rt - kt);
}
