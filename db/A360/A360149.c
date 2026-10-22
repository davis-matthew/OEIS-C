/*
 * A360149  a(n) = Sum_{k=0..floor(n/2)} binomial(2*n+k,n-2*k).
 * https://oeis.org/A360149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*n+k, n-2*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n >> 1;          /* floor(n/2) */
    for (idx_t k = 0; k <= max_k; ++k) {
        sum += binomial(2 * n + k, n - 2 * k);
    }
    return sum;
}
