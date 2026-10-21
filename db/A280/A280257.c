/*
 * A280257  Numbers k such that tau(k^(k-1)) is a prime.
 * https://oeis.org/A280257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 4741, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, is_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280257: numbers k such that tau(k^(k-1)) is prime.
   Computed by testing primes and prime powers. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* is_prime(idx_t) */
#include "../utils/is_prime_small.h"    /* is_prime_small(seq_t) */
#include "../utils/_smallest_prime_factor.h" /* _smallest_prime_factor(uint64_t) */

seq_t a(idx_t n)
{
    idx_t k = 2;         /* first candidate (k >= 2) */
    idx_t found = 0;     /* how many valid numbers have been found */

    while (found < n)
    {
        bool ok;

        /* Every prime is a term. */
        if (is_prime(k))
        {
            ok = true;
        }
        else
        {
            /* Check if k is a pure prime power p^e (e >= 2). */
            uint64_t spf = _smallest_prime_factor(k);
            uint64_t tmp = k;
            unsigned e = 0;
            while (tmp % spf == 0)
            {
                tmp /= spf;
                ++e;
            }

            if (tmp != 1)   /* not a single prime power */
            {
                ok = false;
            }
            else
            {
                /* For k = p^e, tau(k^(k-1)) = e*(k-1)+1. */
                seq_t val = (seq_t)e * ((seq_t)k - 1) + 1;
                ok = is_prime_small(val);
            }
        }

        if (ok)
        {
            ++found;
            if (found == n)
                return (seq_t)k;
        }

        ++k;
    }

    /* This point should never be reached. */
    return 0;
}
