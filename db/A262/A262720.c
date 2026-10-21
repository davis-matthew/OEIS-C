/*
 * A262720  a(n) = Sum_{k=0..n/2} binomial(n+3,k)*binomial(n+1-k,k+1).
 * https://oeis.org/A262720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n+3, k) * binomial(n+1-k, k+1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 2;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term1 = binom(n + 3, k);
        seq_t term2 = binom(n + 1 - k, k + 1);
        sum += term1 * term2;
    }
    return sum;
}
