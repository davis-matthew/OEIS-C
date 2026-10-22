/*
 * A383025  Centered pentagonal numbers that are deficient.
 * https://oeis.org/A383025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = centered pentagonal number P(m) = (5m²-5m+2)/2 that * is
 *            deficient (sum of proper divisors < P(m)). * The sequence lists
 *            those P(m) in ascending order of m that satisfy * the deficiency
 *            condition
 * Range:     u8 n <= 9, u16 n <= 132, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Compute sigma (sum of all divisors) of a value using the prime list. */
static seq_t sigma_of(seq_t num, const uint64_t *primes, size_t primes_cnt)
{
    seq_t sigma = 1;
    uint64_t temp = (uint64_t)num;

    for (size_t i = 0; i < primes_cnt &&
                       (uint64_t)primes[i] * (uint64_t)primes[i] <= temp;
         ++i)
    {
        uint64_t p = primes[i];
        if (temp % p == 0)
        {
            seq_t sum = 1, term = 1;
            while (temp % p == 0)
            {
                temp /= p;
                term *= (seq_t)p;
                sum += term;
            }
            sigma *= sum;
        }
    }

    if (temp > 1)
        sigma *= (seq_t)temp + 1;     /* remaining prime factor */

    return sigma;
}

seq_t a(idx_t n)
{
    /* Static state for caching results and reusing primes. */
    static seq_t *cache   = NULL;   /* array of already computed terms */
    static size_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;    /* allocated capacity of cache */
    static idx_t next_m = 1;        /* next m (index of centered pentagonal number) to test */

    static uint64_t *primes   = NULL;   /* list of primes for factorisation */
    static size_t primes_cnt = 0;       /* number of primes in the list */

    /* Initialise the prime list once. 200 000 is well above the
     * required sqrt(5·n²) for n up to 10 000. */
    if (!primes)
        build_primes_upto(200000, &primes, &primes_cnt);

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        seq_t m  = (seq_t)next_m;
        seq_t cp = ((5 * m * m) - (5 * m) + 2) / 2;   /* centered pentagonal number */

        /* Test for deficiency: sigma(cp) < 2·cp */
        seq_t sigma = sigma_of(cp, primes, primes_cnt);
        if (sigma < (cp << 1))
        {
            /* Grow the cache if necessary. */
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache)   /* allocation failure – return zero */
                    return (seq_t)0;
                cache = new_cache;
                cache_cap = new_cap;
            }

            cache[cache_len++] = cp;   /* store the new term */
        }

        ++next_m;   /* advance to next centered pentagonal number */
    }

    return cache[(size_t)n - 1];
}
