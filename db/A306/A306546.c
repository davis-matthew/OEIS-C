/*
 * A306546  Modified Collatz Map such that odd numbers are treated the same,
 *          but even numbers have all factors of 2 removed.
 * https://oeis.org/A306546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for any even n, a(n) = A000265(n); for any odd n, a(n) = 3*n + 1
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* convert index to sequence type */
    if ((x & 1) == 0)            /* even n */
        return odd_part(x);
    else                         /* odd n */
        return (seq_t)3 * x + 1;
}
