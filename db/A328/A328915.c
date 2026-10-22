/*
 * A328915  If n = Product (p_j^k_j) then a(n) = Product (nextprime(p_j)),
 *          where nextprime = A151800.
 * https://oeis.org/A328915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of nextprime(p) over all distinct prime factors p
 *            of n (OEIS A328915)
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

/* Compute the value of a(n) as defined above.
   seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* For n <= 1 the product over an empty set is defined as 1. */
    if (n <= 1)
        return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Trial division to extract all distinct prime factors. */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t np = next_prime(p);
            result *= (seq_t)np;

            /* Divide out all copies of p so we only process each prime once. */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* If temp > 1, then temp itself is a prime > sqrt(original n). */
    if (temp > 1) {
        uint64_t np = next_prime(temp);
        result *= (seq_t)np;
    }

    return result;
}
