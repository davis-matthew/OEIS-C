/*
 * A361522  The aerated factorial numbers.
 * https://oeis.org/A361522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is odd, otherwise a(n) = (n/2)!
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 25, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n & 1ULL)
        return (seq_t)0;
    return factorial(n >> 1);
}
