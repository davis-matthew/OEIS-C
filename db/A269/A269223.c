/*
 * A269223  Factorial of the sum of digits of n in base 3.
 * https://oeis.org/A269223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = factorial( sum of base-3 digits of n )
 * Range:     u8 n <= 25, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    idx_t tmp = n;
    idx_t sum = 0;
    while (tmp)
    {
        sum += tmp % 3;
        tmp /= 3;
    }
    return factorial(sum);
}
