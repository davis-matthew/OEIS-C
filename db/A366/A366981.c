/*
 * A366981  Number of divisors of n in the set {3,4,5}.
 * https://oeis.org/A366981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [3|n] + [4|n] + [5|n] (number of divisors of n that are 3
 *            or 4 or 5)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n % 3 == 0) count += (seq_t)1;
    if (n % 4 == 0) count += (seq_t)1;
    if (n % 5 == 0) count += (seq_t)1;
    return count;
}
