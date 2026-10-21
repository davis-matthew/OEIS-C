/*
 * A289493  Number of primes in the interval [2n, 3n].
 * https://oeis.org/A289493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(3n) - pi(2n) – number of primes in the interval
 *            [2n,3n]
 * Range:     u8 n <= 2219, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Sequential (non‑recursive) sieve of Eratosthenes.
   The routine keeps a static cache of all primes up to the largest
   bound requested so far.  Subsequent calls reuse the cache.
   The function is thread‑unsafe, as required for all build utilities. */

seq_t a(idx_t n)
{
    if (n < 1)  /* sequence is defined only for n ≥ 1 */
        return (seq_t)0;

    /* Static state for the prime cache */
    static idx_t sieve_limit = 0;   /* largest bound sieved so far   */
    static uint64_t *primes       = NULL;   /* table of primes             */
    static size_t  primes_cnt     = 0;       /* number of primes in table   */

    /* Current required upper bound */
    idx_t needed = 3 * n;
    if (needed > sieve_limit)
    {
        /* Allocate the boolean array for the sieve */
        size_t sz = (size_t)needed + 1;
        uint8_t *is_prime = (uint8_t *)calloc(sz, sizeof(uint8_t));
        if (!is_prime)              /* allocation failure – return 0 */
            return (seq_t)0;

        /* Mark all numbers ≥ 2 as potentially prime */
        for (size_t i = 2; i < sz; ++i)
            is_prime[i] = 1;

        /* Eratosthenes sieve */
        for (size_t i = 2; i * i <= (size_t)needed; ++i)
        {
            if (is_prime[i])
            {
                for (size_t j = i * i; j < sz; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Count primes to allocate the primes array */
        size_t new_cnt = 0;
        for (size_t i = 2; i < sz; ++i)
            if (is_prime[i])
                ++new_cnt;

        uint64_t *new_primes = (uint64_t *)malloc(new_cnt * sizeof(uint64_t));
        if (!new_primes)            /* allocation failure – cleanup & return 0 */
        {
            free(is_prime);
            return (seq_t)0;
        }

        /* Fill the primes array */
        size_t idx = 0;
        for (size_t i = 2; i < sz; ++i)
            if (is_prime[i])
                new_primes[idx++] = (uint64_t)i;

        free(is_prime);

        /* Replace old cache with the new one */
        free(primes);
        primes   = new_primes;
        primes_cnt = new_cnt;
        sieve_limit = needed;
    }

    /* Count primes in the interval [2n,3n] */
    idx_t lo = 2 * n;
    idx_t hi = 3 * n;
    seq_t count = 0;

    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p < lo)
            continue;
        if (p > hi)
            break;
        ++count;
    }
    return count;
}
