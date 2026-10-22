/*
 * A325070  Prime numbers congruent to 9 modulo 16 representable by x^2 +
 *          64*y^2.
 * https://oeis.org/A325070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A325070: prime numbers p ≡ 9 (mod 16) that can be written as
   x^2 + 64*y^2.  The routine searches sequentially for the n‑th such
   prime; no caching is used.  The algorithm is applicable for all
   supported seq_t types, including arbitrary-precision integers. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"

/* Test whether a prime p satisfies p = x^2 + 64*y^2. */
static bool representable_by_64(seq_t p)
{
    seq_t max_y = floor_sqrt(p / 64);
    for (seq_t y = 0; y <= max_y; ++y) {
        seq_t term = 64 * y * y;
        seq_t rem  = p - term;
        seq_t sq   = floor_sqrt(rem);
        if (sq * sq == rem)
            return true;
    }
    return false;
}

/* ---------------------------------------------------------------
   Return the n‑th term of A325070 (1‑based index).
   --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* behavior below offset is irrelevant */

    idx_t   found   = 0;          /* how many valid primes we have found */
    seq_t   candidate = 2;        /* start with the smallest prime          */

    while (found < n) {
        /* Test candidate for primality */
        if (is_prime_big(candidate)) {
            /* Check congruence condition */
            if ((candidate % 16) == 9) {
                /* Check representation condition */
                if (representable_by_64(candidate)) {
                    found++;
                    if (found == n)
                        return candidate;
                }
            }
        }

        /* Advance to next candidate:
           after 2, only odd numbers are possible */
        candidate = (candidate == 2) ? 3 : candidate + 2;
    }

    /* Should never reach here */
    return 0;
}
