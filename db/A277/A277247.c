/*
 * A277247  a(n) = Sum_{k=0..floor(n/2)} binomial(n, k)^2.
 * https://oeis.org/A277247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n,k)^2 Computed as a(n) = (
 *            binomial(2n,n) + ( binomial(n,n/2) * cos(πn/2) )^2 ) / 2 For
 *            integer arithmetic this becomes a(n) = ( binomial(2n,n) + ( (n
 *            even) ? binomial(n,n/2)^2 : 0 ) ) / 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t res = binom(2 * n, n);

    if ((n & 1U) == 0) {
        seq_t mid = binom(n, n >> 1);
        res = res + (mid * mid);
    }

    return res / 2;
}
