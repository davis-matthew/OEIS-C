/*
 * A249376  Prime numbers Q such that the concatenation Q,3,Q is prime.
 * https://oeis.org/A249376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   primes Q such that the concatenation Q,3,Q is prime
 * Range:     u8 none, u16 n <= 8, u32 n <= 728, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, grow_seq, isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a static cache of found terms and
   uses generic primality testing.  It scans odd numbers, checks
   primality with isPrimeGeneric, builds the concatenated number
   using pow10 and digit_count, tests that for primality, and
   stores each valid Q.  The cache is grown lazily with grow_seq.  */

#include <stddef.h>      /* NULL, size_t */
#include <stdbool.h>     /* bool */

#include "../utils/grow_seq.h"
#include "../utils/digit_count.h"
#include "../utils/pow10.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Static state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t candidate = 3;  /* start from the first odd prime */

    /* Grow the cache until we have the nth term */
    while (cache_len < (size_t)n) {
        /* Find the next prime candidate */
        while (!isPrimeGeneric((seq_t)candidate))
            candidate += 2;

        /* candidate is now prime Q */
        uint64_t q = candidate;
        seq_t q_seq = (seq_t)q;

        /* Number of decimal digits of Q */
        uint64_t d = digit_count(q);

        /* 10^d and 10^(d+1) */
        seq_t pow_d  = pow10(d);
        seq_t pow_d1 = pow10(d + 1);

        /* Concatenate Q,3,Q:  Q*10^(d+1) + 3*10^d + Q  */
        seq_t concat = q_seq * pow_d1 + 3 * pow_d + q_seq;

        /* If the concatenated number is prime, store Q */
        if (isPrimeGeneric(concat)) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = q_seq;
        }

        /* Advance to the next odd number */
        candidate += 2;
    }

    return cache[(size_t)n - 1];
}
