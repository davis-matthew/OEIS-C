/*
 * A259102  Composite repunits with a prime number of 1's.
 * https://oeis.org/A259102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth composite repunit whose length is prime, i.e.
 *            a(n) = (10^p - 1)/9 where p is the nth prime such that the
 *            repunit of length p is composite
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime, is_prime_big, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 2;

    while (cache_len < n)
    {
        /* Find the next prime candidate */
        while (!is_prime(next_candidate))
            ++next_candidate;

        /* Compute the repunit of length next_candidate */
        seq_t rep = pow_uint(10, next_candidate);
        rep = rep - 1;
        seq_t repunit = rep / (seq_t)9;

        /* Store if composite */
        if (!is_prime_big(repunit))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();  /* memory allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = repunit;
        }
        ++next_candidate;  /* Proceed to the next integer */
    }
    return cache[n - 1];
}
