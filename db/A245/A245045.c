/*
 * A245045  Primes of the form (k^2+2)/6.
 * https://oeis.org/A245045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 70, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245045: primes of the form (k^2+2)/6 */

/* The routine keeps a static cache of the primes found so far.
   On demand it expands the cache by searching successive k values
   that satisfy (k^2+2) divisible by 6 (i.e. k ≡ 2 or 4 mod 6) and
   retaining those for which (k^2+2)/6 is prime. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* never requested for n = 0; such a call is ignored */
    if (n == 0) return 0;

    /* static cache holds the primes in order */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;   /* number of valid entries in cache */
    static idx_t  last_k = 3;      /* last k inspected (next will be 4) */

    /* Grow the cache until it contains at least n primes */
    if (n > cache_len)
    {
        /* Ensure capacity for at least n entries */
        if (n > cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < n) new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        while (cache_len < n)
        {
            ++last_k;
            /* Only k ≡ 2 or 4 (mod 6) give integer (k^2+2)/6 */
            if ((last_k % 6) != 2 && (last_k % 6) != 4)
                continue;

            seq_t ksq = (seq_t)last_k * (seq_t)last_k;
            seq_t p   = (ksq + 2) / 6;

            if (is_prime_big(p))
            {
                cache[cache_len++] = p;
            }
        }
    }

    return cache[n - 1];
}
