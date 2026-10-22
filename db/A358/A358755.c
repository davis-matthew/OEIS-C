/*
 * A358755  a(n) = 1 if A053669(n) [the smallest prime not dividing n] is of
 *          the form 6m-1, otherwise a(n) = 0.
 * https://oeis.org/A358755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 1 if the smallest prime not dividing n is congruent to
 *            5 modulo 6, * otherwise a(n) = 0. * * The function searches for
 *            the least prime p such that n % p != 0 * and returns 1 when p ≡
 *            5 (mod 6) and 0 otherwise. * A minimal prime test is provided by
 *            the utility function * is_prime_small(se...
 * Range:     u8 n <= 9189, u16 n <= 9189, u32 n <= 9189, u64 n <= 9189,
 *            u128 n <= 9189, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;   /* index conversion */

    /* iterate through all integers starting from 2,
       checking primality and divisibility by n. */
    for (uint64_t p = 2; ; ++p)
    {
        if (is_prime_small((seq_t)p))
        {
            if (nn % p != 0u)            /* p does not divide n */
                return (seq_t)((p % 6u) == 5u);
        }
    }
}
