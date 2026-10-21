/*
 * A282098  a(n) = A006530(a(n-1)) + A006530(a(n-2)) + A006530(a(n-3)) with
 *          a(0) = a(1) = a(2) = 1.
 * https://oeis.org/A282098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = A006530(a(n-1)) + A006530(a(n-2)) + A006530(a(n-3)), a(0)
 *            = a(1) = a(2) = 1 where A006530(x) is the largest prime factor
 *            of x (returns 1 for x <= 1)
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   max_prime_factor_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/max_prime_factor_of.h"

/* Main sequence function.  Uses a static cache that grows as needed.
   The code is valid C11 and C++17. */
seq_t
a(idx_t n)
{
    /* static cache: values a(0) .. a(max_computed) */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static idx_t   max_comp  = (idx_t)-1;   /* index of last computed term */

    /* handle small indices directly */
    if (n < 3)
        return (seq_t)1;

    /* initialise cache lazily */
    if (cache == NULL) {
        cache_cap = 4;          /* initial capacity (at least 3+1) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            abort();
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        max_comp = 2;
    }

    /* grow and compute up to the requested index */
    if (n > max_comp) {
        /* increase capacity if necessary */
        if ((size_t)(n + 1) > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 4;
            while (new_cap <= (size_t)(n + 1))
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)
                abort();
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            seq_t v1 = max_prime_factor_of(cache[(size_t)(i - 1)]);
            seq_t v2 = max_prime_factor_of(cache[(size_t)(i - 2)]);
            seq_t v3 = max_prime_factor_of(cache[(size_t)(i - 3)]);
            cache[(size_t)i] = v1 + v2 + v3;
        }
        max_comp = n;
    }

    return cache[(size_t)n];
}
