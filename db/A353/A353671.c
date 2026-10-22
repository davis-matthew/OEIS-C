/*
 * A353671  a(n) = 1 if n is even and the odd part of n is a prime, otherwise
 *          0.
 * https://oeis.org/A353671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is even and the odd part of n is a prime, else 0
 * Range:     u8 n <= 521, u16 n <= 9608, u32 n <= 9608, u64 n <= 9608,
 *            u128 n <= 9608, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of sequence */
    if ((n & 1) != 0)          /* odd n -> 0 */
        return (seq_t)0;

    /* extract odd part of n */
    idx_t odd = n;
    while ((odd & 1ULL) == 0)
        odd >>= 1;

    /* odd part is odd; test primality */
    if (is_prime_big((seq_t)odd))
        return (seq_t)1;
    else
        return (seq_t)0;
}
