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
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t res = binomial(2 * n, n);

    if ((n & 1U) == 0) {
        seq_t mid = binomial(n, n >> 1);
        res = res + (mid * mid);
    }

    return res / 2;
}
