/*
 * A354975  a(n) = Sum_{i=1..n} (prime(i+n) mod prime(i)).
 * https://oeis.org/A354975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (prime(i+n) mod prime(i))
 * Range:     u8 n <= 18, u16 n <= 214, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        uint32_t p1 = get_prime(i);
        uint32_t p2 = get_prime(i + n);
        sum += (seq_t)(p2 % p1);
    }
    return sum;
}
