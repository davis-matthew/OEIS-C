/*
 * A241029  Sum of n-th powers of divisors of 22.
 * https://oeis.org/A241029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 2^n) * (1 + 11^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow2  = big_pow((seq_t)2,  (seq_t)n);
    seq_t pow11 = big_pow((seq_t)11, (seq_t)n);

    seq_t sum2  = (seq_t)1 + pow2;
    seq_t sum11 = (seq_t)1 + pow11;

    return sum2 * sum11;
}
