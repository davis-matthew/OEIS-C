/*
 * A294233  Number of partitions of n into two parts with smaller part
 *          nonsquarefree and larger part squarefree.
 * https://oeis.org/A294233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..⌊n/2⌋} μ(n-i)² · (1 – μ(i)²)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t half = n / 2;
    for (idx_t i = 1; i <= half; ++i)
    {
        seq_t mu_i = mobius(i);
        seq_t mu_j = mobius(n - i);
        /* μ² == 1 if squarefree, 0 otherwise */
        seq_t mu_i_sq = (mu_i == 0 ? 0 : 1);
        seq_t mu_j_sq = (mu_j == 0 ? 0 : 1);
        if (mu_j_sq == 1 && mu_i_sq == 0)
            result += 1;
    }
    return result;
}
