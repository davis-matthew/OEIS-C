/*
 * A383525  a(n) = Sum_{k=0..floor(n/3)} binomial(n,k)^2 * binomial(n-2*k,k).
 * https://oeis.org/A383525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n,k)^2 * binomial(n-2*k,k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 3; ++k) {
        seq_t b1 = binomial(n, k);
        seq_t b1_sq = b1 * b1;
        seq_t b2 = binomial(n - 2 * k, k);
        sum += b1_sq * b2;
    }
    return sum;
}
