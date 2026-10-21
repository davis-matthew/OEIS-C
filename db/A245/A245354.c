/*
 * A245354  Sum of digits of n in fractional base 9/5.
 * https://oeis.org/A245354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007953(A024653(n))
 * Range:     u8 n <= 279, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of the digits of n in the fractional base 9/5. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = (seq_t)n;
    seq_t sum = 0;
    while (r != (seq_t)0)
    {
        seq_t digit = r % 9;          /* least significant digit */
        sum += digit;
        /* Next value: (r - digit) * 5 / 9, guaranteed to be integer. */
        r = (r - digit) * 5 / 9;
    }
    return sum;
}
