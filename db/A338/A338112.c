/*
 * A338112  Least number that is both the sum and product of n distinct
 *          positive integers.
 * https://oeis.org/A338112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! for n = 1 and n > 2; a(2) = 3
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 2)
        return (seq_t)3;
    return factorial(n);
}
