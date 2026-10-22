/*
 * A353670  a(n) = 1 if the odd part of n is a prime, otherwise 0.
 * https://oeis.org/A353670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the odd part of n is prime, otherwise 0
 * Range:     u8 n <= 9632, u16 n <= 9632, u32 n <= 9632, u64 n <= 9632,
 *            u128 n <= 9632, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t odd = n;
    while ((odd & 1ULL) == 0)
        odd >>= 1;

    return is_prime(odd) ? 1 : 0;
}
