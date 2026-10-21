/*
 * A270003  Least prime p such that n = p + q - r for some primes q and r with
 *          q > p.
 * https://oeis.org/A270003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 if n is odd nonprime, 2 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Return the least prime p such that n = p + q - r with q > p.
       It has been proved that p = 2 for all n except the odd
       non‑prime numbers, where p = 3. */
    if ((n & 1ULL) && !is_prime(n))
        return 3;
    else
        return 2;
}
