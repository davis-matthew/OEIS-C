/*
 * A294235  Number of partitions of n into two parts such that the larger part
 *          is nonsquarefree.
 * https://oeis.org/A294235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (1 - mu(n-i)^2)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n / 2;
    for (idx_t i = 1; i <= half; ++i) {
        idx_t j = n - i;
        seq_t mu = mobius(j);
        sum += 1 - mu * mu;
    }
    return sum;
}
