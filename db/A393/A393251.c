/*
 * A393251  a(n) = Sum_{k=0..floor(n/6)} binomial(2*k,k) *
 *          binomial(n-4*k,2*k).
 * https://oeis.org/A393251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/6)} binomial(2*k,k) *
 *            binomial(n-4*k,2*k)
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 47, u64 n <= 89, u128 n <= 174,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 6;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(2 * k, k);
        seq_t t2 = binomial(n - 4 * k, 2 * k);
        sum += t1 * t2;
    }
    return sum;
}
