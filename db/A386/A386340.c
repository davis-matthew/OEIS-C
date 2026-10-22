/*
 * A386340  Primes without {3, 4} as digits.
 * https://oeis.org/A386340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1736, u32 n <= 1737, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth prime that contains no digits 3 or 4. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static bool digits_ok(seq_t x)
{
    while (x > 0)
    {
        unsigned d = (unsigned)(x % 10);
        if (d == 3 || d == 4)
            return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Ensure n >= 1. Behaviour for n==0 is undefined as per spec. */
    static seq_t *cache = NULL;     /* cached primes */
    static size_t cache_len = 0;    /* number of cached primes */
    static size_t cache_cap = 0;    /* allocated capacity */
    static seq_t next_candidate = 2;/* next number to test */

    while (cache_len < n)
    {
        if (digits_ok(next_candidate) && is_prime_big(next_candidate))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
                if (!tmp)
                    exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
