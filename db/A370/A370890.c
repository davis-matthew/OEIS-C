/*
 * A370890  A(n, k) = 2^n*Pochhammer(k/2, floor((n+1)/2)). Square array read
 *          by ascending antidiagonals.
 * https://oeis.org/A370890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the array A(n,k) defined by * A(n,k) =
 *            2^n * Pochhammer(k/2, floor((n+1)/2)) and read in * ascending
 *            antidiagonals (row n, column k)
 * Range:     u8 n <= 36, u16 n <= 78, u32 n <= 171, u64 n <= 465,
 *            u128 n <= 1275, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the diagonal index s such that
     *     (s+1)(s+2)/2 > n  and (s)(s+1)/2 <= n.
     */
    idx_t s = 0;
    __uint128_t cum = 1;           /* cumulative terms up to diagonal s */
    while (cum <= n) {
        ++s;
        cum += (s + 1);            /* add terms of the next diagonal      */
    }

    /* Number of terms before this diagonal */
    __uint128_t prev_tmp = (__uint128_t)s * (s + 1) / 2;
    idx_t prev_total = (idx_t)prev_tmp;

    /* Position within the diagonal */
    idx_t r   = n - prev_total;    /* 0‑based offset on the diagonal     */
    idx_t row = s - r;             /* row index in the array             */
    idx_t col = r;                 /* column index in the array          */

    /* Compute m = floor((row+1)/2) and the product */
    idx_t m = (row + 1) / 2;       /* number of factors in the pochhammer  */

    seq_t result = 1;
    for (idx_t k = 0; k < m; ++k) {
        result = result * (seq_t)(col + 2 * k);
    }

    /* Multiply by the remaining power of two: 2^(row-m) */
    result = result * pow2(row - m);

    return result;
}
