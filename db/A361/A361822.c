/*
 * A361822  Primes without {2, 5} as digits.
 * https://oeis.org/A361822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 2334, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose decimal digits exclude 2 and 5
   (OEIS A361822).  The function generates successive primes
   and keeps only those that do not contain the digits 2
   or 5, returning the n‑th such prime.  */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* `cur` scans over natural numbers, `cnt` counts qualifying primes. */
    idx_t cur   = 1;
    idx_t cnt   = 0;
    seq_t val   = 0;

    while (cnt < n)
    {
        cur++;

        /* Test primality first (fast for numbers < 2^64). */
        if (!is_prime(cur))
            continue;

        /* Reject primes that contain a 2 or a 5 as a decimal digit. */
        idx_t t = cur;
        bool ok = true;
        while (t > 0)
        {
            uint32_t d = t % 10;
            if (d == 2 || d == 5)
            {
                ok = false;
                break;
            }
            t /= 10;
        }
        if (!ok)
            continue;

        /* Accept this prime. */
        cnt++;
        if (cnt == n)
            val = (seq_t)cur;
    }

    return val;
}
