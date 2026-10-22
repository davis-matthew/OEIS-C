/*
 * A387477  a(n) = Sum_{k=0..floor(n/3)} 2^k * binomial(k,n-3*k)^2.
 * https://oeis.org/A387477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} 2^k * binomial(k,n-3*k)^2
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 42, u64 n <= 79, u128 n <= 153,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t r = n - 3*k;
        if (r > k) continue;                   // binomial(k,r)=0 otherwise
        seq_t b = binomial(k, r);
        seq_t sq = b * b;
        seq_t term = pow2(k) * sq;
        sum += term;
    }
    return sum;
}
