/*
 * A308160  Take all the integer-sided isosceles triangles with perimeter n
 *          and sides a, b, and c such that a <= b <= c. a(n) is the sum of
 *          all the b's.
 * https://oeis.org/A308160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{s = floor(n/4)+1}^{floor(n/3)} s + sum_{t =
 *            floor(n/3)+1}^{floor((n-1)/2)} t (a(1) = 0)
 * Range:     u8 n <= 52, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute ranges for the two families of isosceles triangles */
    idx_t L1 = (n / 4) + 1;          /* lower bound for a=b case */
    idx_t U1 = n / 3;                /* upper bound for a=b case */

    idx_t L2 = (n / 3) + 1;          /* lower bound for a<b=c case (strict inequality) */
    idx_t U2 = (n - 1) / 2;          /* upper bound for a<b=c case */

    seq_t sum = 0;

    if (L1 <= U1) {                  /* case a = b */
        seq_t count = (seq_t)(U1 - L1 + 1);
        sum += ((seq_t)L1 + (seq_t)U1) * count / 2;
    }

    if (L2 <= U2) {                  /* case a < b = c */
        seq_t count = (seq_t)(U2 - L2 + 1);
        sum += ((seq_t)L2 + (seq_t)U2) * count / 2;
    }

    return sum;
}
