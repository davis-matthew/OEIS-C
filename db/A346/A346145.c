/*
 * A346145  Primes of the form k^2 + 25.
 * https://oeis.org/A346145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form k²+25, with k an even positive
 *            integer
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates primes by successive testing of k²+25 for primality
   and memoises the results so that arbitrary calling order of a(n) works efficiently. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t count  = 0;   /* number of primes found so far */
    static idx_t last_k = 0;   /* last k processed (even integer) */

    /* ensure we have at least n terms */
    while (count < n)
    {
        /* next even k */
        last_k += 2;

        /* p = k² + 25 */
        seq_t p = (seq_t)last_k * (seq_t)last_k + (seq_t)25;

        /* test primality; p fits in uint64_t because k is tiny (≤~2000 for n≈10 000) */
        if (is_prime_small((uint64_t)p))
        {
            /* grow cache if necessary */
            if (count >= cap)
            {
                size_t new_cap = cap == 0 ? 64 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)  /* allocation failed */
                {
                    abort();
                }
                cache = tmp;
                cap   = new_cap;
            }
            cache[count++] = p;
        }
    }

    return cache[n - 1];
}
