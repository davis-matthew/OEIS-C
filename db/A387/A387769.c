/*
 * A387769  a(n) = Sum_{k=0..floor(n/3)} 2^k * binomial(2*k+1,2*n-6*k+1).
 * https://oeis.org/A387769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^k * binomial(2*k+1, 2*n-6*k+1)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 75, u128 n <= 149,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t sum = 0;
    seq_t power_of_two = 1;   /* 2^0 */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t n1 = 2 * k + 1;
        idx_t k1 = 2 * n - 6 * k + 1;

        seq_t bin = binomial(n1, k1); /* returns 0 if k1 > n1 */
        sum += power_of_two * bin;

        power_of_two <<= 1;   /* multiply by 2 for next iteration */
    }

    return sum;
}
