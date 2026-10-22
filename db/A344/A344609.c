/*
 * A344609  Numbers whose alternating sum of prime indices is >= 0.
 * https://oeis.org/A344609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer whose alternating sum of prime
 *            indices of its factorization is >= 0
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Static cache of terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Sieve state */
    static idx_t *spf = NULL;        /* smallest prime factor table */
    static idx_t *primes = NULL;     /* prime list */
    static size_t primes_count = 0;
    static unsigned *prime_index = NULL; /* maps prime value to its 1‑based index */
    static idx_t limit = 0;          /* current sieve limit */
    static idx_t current = 1;        /* next integer to test */

    /* Grow cache when needed */
    if (cache_len == 0)
    {
        cache_cap = 64;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n)
    {
        /* Build or extend sieve when necessary */
        if (current > limit)
        {
            idx_t new_limit = (limit == 0) ? 2 : limit << 1;
            while (new_limit < current)
                new_limit <<= 1;

            /* Free old tables */
            if (spf)      free(spf);
            if (primes)   free(primes);
            if (prime_index) free(prime_index);

            /* Build new tables */
            build_spf(new_limit, &spf);
            build_primes_upto(new_limit, &primes, &primes_count);

            /* Build prime→index map */
            prime_index = (unsigned *)calloc((size_t)new_limit + 1, sizeof(unsigned));
            for (size_t i = 0; i < primes_count; ++i)
                prime_index[primes[i]] = (unsigned)(i + 1);   /* 1‑based index */

            limit = new_limit;
        }

        /* Compute alternating sum of prime indices for 'current' */
        idx_t temp = current;
        long long alt_sum = 0;
        int sign = 1;   /* +1, -1 pattern */
        while (temp > 1)
        {
            idx_t p = spf[temp];
            unsigned idx = prime_index[p];
            alt_sum += sign * (long long)idx;
            sign = -sign;
            temp /= p;
        }

        /* If condition satisfied, store the term */
        if (alt_sum >= 0)
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len] = (seq_t)current;
            ++cache_len;
        }

        ++current;
    }

    return cache[n - 1];
}
