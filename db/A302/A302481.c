/*
 * A302481  Number of partitions of n into two parts with the smaller part
 *          prime and the larger part nonprime.
 * https://oeis.org/A302481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} prime(i) * (1 - prime(n-i))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of n into two parts where the smaller part is prime and the larger part is nonprime. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t ans = 0;
    idx_t half = n / 2;
    for (idx_t i = 1; i <= half; ++i)
    {
        if (is_prime(i) && !is_prime(n - i))
        {
            ans++;
        }
    }
    return ans;
}
