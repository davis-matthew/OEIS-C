/*
 * A400847  Primes p such that (p^2 + nextprime(p))/2 is also prime.
 * https://oeis.org/A400847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, new
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 7, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A400847: primes p such that (p^2 + nextprime(p))/2 is prime. */
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; behaviour for n < 1 is irrelevant */
        return 0;

    idx_t cur     = 3;            /* first candidate prime */
    idx_t found   = 0;

    while (found < n)
    {
        idx_t nextp = next_prime(cur);                /* next prime after cur */

        /* ((cur^2 + nextp) / 2, computed in seq_t without overflow) */
        seq_t sq      = (seq_t)cur * cur;
        seq_t sum     = sq + (seq_t)nextp;
        seq_t term    = sum / 2;

        if (is_prime_big(term))
        {
            ++found;
            if (found == n)
                return (seq_t)cur;  /* nth qualifying prime */
        }

        cur = nextp;   /* advance to next candidate prime */
    }

    /* control never reaches here, but return 0 to keep compiler happy */
    return 0;
}
