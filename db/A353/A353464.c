/*
 * A353464  Characteristic function of greater twin primes: a(n) = 1 if both n
 *          and n-2 are primes, otherwise 0.
 * https://oeis.org/A353464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010051(n) * A010051(n-2)
 * Range:     u8 n <= 9774, u16 n <= 9774, u32 n <= 9774, u64 n <= 9774,
 *            u128 n <= 9774, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;
    return (seq_t)(is_prime(n) && is_prime(n - 2));
}
