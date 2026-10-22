/*
 * A354074  Factorials that are the sum of the divisors of some m.
 * https://oeis.org/A354074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = factorial(n+1) for n ≥ 2 (equivalently all
 *            factorials except 2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    /* n ≥ 2: return (n+1)! which is the next factorial after 2! */
    return factorial(n + 1);
}
