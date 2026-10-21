/*
 * A283149  Largest k such that (p-1)! == -1 (mod p^k), where p = prime(n).
 * https://oeis.org/A283149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if prime(n) is a Wilson prime (5, 13, 563), otherwise
 *            1. Wilson primes are known only for these three primes
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"

static const uint32_t known_wilson_primes[] = {5U, 13U, 563U};

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  We return 0 for n == 0,
       though this case is never used. */
    if (n == 0U)
        return (seq_t)0;

    /* First prime in the sequence is 2. */
    uint32_t p = get_prime(n);

    /* Check whether p is one of the known Wilson primes. */
    size_t i, len = sizeof(known_wilson_primes) / sizeof(known_wilson_primes[0]);
    for (i = 0; i < len; ++i)
    {
        if (p == known_wilson_primes[i])
            return (seq_t)2;
    }

    return (seq_t)1;
}
