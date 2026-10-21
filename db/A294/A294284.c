/*
 * A294284  Sum of the smaller parts of the partitions of n into two distinct
 *          parts with larger part squarefree.
 * https://oeis.org/A294284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} i * mu(n-i)^2
 * Range:     u8 n <= 55, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = (n - 1) / 2;
    for (idx_t i = 1; i <= limit; ++i)
    {
        idx_t larger = n - i;
        if (is_squarefree((seq_t)larger))
            sum += (seq_t)i;
    }
    return sum;
}
