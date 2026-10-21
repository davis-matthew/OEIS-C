/*
 * A251561  A permutation of the natural numbers: interchange p and 2p for
 *          every prime p.
 * https://oeis.org/A251561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { 2*n if n is prime, n/2 if n is twice a prime, otherwise
 *            n }
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (is_prime(n))
        return (seq_t)n * 2;
    if ((n & 1) == 0) {
        idx_t p = n >> 1;
        if (is_prime(p))
            return (seq_t)p;
    }
    return (seq_t)n;
}
