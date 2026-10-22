/*
 * A335808  Nonzero multiplicative persistence in base 10: number of
 *          iterations of "multiply nonzero digits in base 10" needed to reach
 *          a number < 10.
 * https://oeis.org/A335808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of iterations of "multiply nonzero digits in base
 *            10" needed to reach a number < 10 (nonzero multiplicative
 *            persistence)
 * Range:     u8 n <= 2094, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   mul_nonzero_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_nonzero_digits.h"

seq_t a(idx_t n)
{
    seq_t num = (seq_t)n;
    if (num < 10)
        return 0;
    seq_t count = 0;
    while (num >= 10)
    {
        num = mul_nonzero_digits(num);
        ++count;
    }
    return count;
}
