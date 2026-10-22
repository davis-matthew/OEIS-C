/*
 * A323295  Number of ways to fill a matrix with the first n positive
 *          integers.
 * https://oeis.org/A323295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n) * n! for n > 0, a(0) = 1, where d(n) = A000005(n) is
 *            the number of divisors of n
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   divisor_count_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == 0)
        return (seq_t)1;

    /* d(n) – number of positive integer divisors of n */
    seq_t d = (seq_t)divisor_count_u64((uint64_t)n);

    /* n! – factorial of n */
    seq_t f = factorial(n);

    /* Product gives the desired sequence value */
    return d * f;
}
