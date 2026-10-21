/*
 * A253969  Primes p such that p + nextprime(p) is divisible by 6.
 * https://oeis.org/A253969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 3916, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: return the n‑th prime p such that p + nextprime(p) is divisible by 6. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of qualifying primes.  The array grows as needed. */
    static seq_t *qual        = NULL;
    static size_t qual_cap    = 0;
    static idx_t   qual_len   = 0;

    /* Helper: prime greater than the last examined one. */
    static uint64_t last_checked = 1;

    /* Generate primes until we have at least n qualifying terms. */
    while (qual_len < n)
    {
        /* Find the next prime after last_checked. */
        uint64_t p = next_prime(last_checked);
        uint64_t q = next_prime(p);

        /* Test the required condition. */
        if ( ((p + q) % 6ULL) == 0ULL )
        {
            /* Resize the cache if necessary. */
            if (qual_len == qual_cap)
            {
                size_t new_cap = qual_cap ? qual_cap * 2 : 64;
                seq_t *tmp = (seq_t*)realloc(qual, new_cap * sizeof(seq_t));
                if (!tmp && new_cap) return (seq_t)0;  /* allocation failure */
                qual = tmp;
                qual_cap = new_cap;
            }

            qual[qual_len++] = (seq_t)p;  /* store the prime in the cache */
        }

        last_checked = p;  /* next search starts after this prime */
    }

    return qual[n - 1];
}
