/*
 * A382841  a(n) = Sum_{k=0..floor(n/2)} (binomial(n,k) * binomial(n-k,k))^2.
 * https://oeis.org/A382841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (binomial(n,k) * binomial(n-k,k))^2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t half = n >> 1;          /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= half; ++k) {
        seq_t b1 = binomial(n, k);           /* C(n,k) */
        seq_t b2 = binomial(n - k, k);       /* C(n-k,k) */
        seq_t prod = b1 * b2;                /* C(n,k)*C(n-k,k) */
        seq_t term = prod * prod;            /* square it */
        sum = sum + term;
    }
    return sum;
}
