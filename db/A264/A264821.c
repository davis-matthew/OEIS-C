/*
 * A264821  Centered 14-gonal (or tetradecagonal) primes.
 * https://oeis.org/A264821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime of the form 7*k^2 - 7*k + 1 (k ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   ensure_capacity, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts with 43, 211, 631, 1093, ... */

#include <stdbool.h>
#include <stddef.h>

#include "../utils/ensure_capacity.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes.
       cache holds the primes in order; len = number of cached primes. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t next_k = 3;   /* next k to test (k=3 gives the first prime) */

    /* We assume n >= 1; behaviour for n < 1 is unspecified. */

    /* Ensure the cache contains at least n primes. */
    while (len < (size_t)n) {
        /* Grow cache if necessary. */
        ensure_capacity(len + 1, &cap, (void **)&cache, sizeof(seq_t));

        /* Compute the candidate prime value. */
        seq_t k_s = (seq_t)next_k;
        seq_t val = 7 * k_s * k_s - 7 * k_s + 1;

        /* Test primality using the provided helper. */
        if (isPrimeGeneric(val)) {
            cache[len++] = val;   /* store if prime */
        }

        ++next_k;                /* try next k in the next iteration */
    }

    /* Return the nth term (1‑based index). */
    return cache[n - 1];
}
