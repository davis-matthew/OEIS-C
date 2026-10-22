/*
 * A365167  Number of representations of n as the sum of a prime number and a
 *          fourth power of a positive integer.
 * https://oeis.org/A365167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as prime + fourth power.
 *            Algorithm: For each n, loop over j from 1 while j^4 <= n-2,
 *            count if n - j^4 is prime
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t count = 0;

    for (idx_t j = 1; ; ++j)
    {
        idx_t pow4 = j * j * j * j;
        if (pow4 > n - 2) break;

        if (is_prime(n - pow4)) ++count;
    }

    return count;
}
