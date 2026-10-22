/*
 * A356173  a(n) = 1 if n is not divisible by two consecutive prime numbers,
 *          otherwise 0.
 * https://oeis.org/A356173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n has no pair of consecutive prime divisors,
 *            otherwise 0
 * Range:     u8 n <= 9611, u16 n <= 9611, u32 n <= 9611, u64 n <= 9611,
 *            u128 n <= 9611, big any n
 * Helpers:   int_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    idx_t limit = int_sqrt(n);
    uint64_t p = 2;
    while (p <= limit) {
        if (((uint64_t)n) % p == 0) {
            uint64_t q = next_prime(p);
            if (((uint64_t)n) % q == 0)
                return (seq_t)0;
        }
        p = next_prime(p);
    }
    return (seq_t)1;
}
