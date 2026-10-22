/*
 * A392033  a(n) = Sum_{k=0..floor(n/2)} binomial(k+3,3) * binomial(n,k) *
 *          binomial(n-k,k).
 * https://oeis.org/A392033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)}
 *            binomial(k+3,3)*binomial(n,k)*binomial(n-k,k)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n >> 1;   /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(k + 3, 3);         /* binomial(k+3,3) */
        seq_t t2 = binomial(n, k);             /* binomial(n,k) */
        seq_t t3 = binomial(n - k, k);         /* binomial(n-k,k) */
        seq_t prod = t1 * t2 * t3;             /* multiply safely */
        sum += prod;
    }
    return sum;
}
