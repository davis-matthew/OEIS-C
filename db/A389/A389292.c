/*
 * A389292  a(n) = Sum_{k=0..floor(n/2)} binomial(n+k-1,k) *
 *          binomial(2*k,n-2*k).
 * https://oeis.org/A389292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n+k-1, k) * binomial(2*k,
 *            n-2*k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 71,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t sum = (seq_t)0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t b1 = binomial(n + k - 1, k);
        seq_t b2 = binomial(2 * k, n - 2 * k);
        sum += b1 * b2;
    }
    return sum;
}
