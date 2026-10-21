/*
 * A253972  Primes p such that p + nextprime(p) is divisible by 8.
 * https://oeis.org/A253972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that p + nextprime(p) is divisible by 8
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache   = NULL;   /* array of found primes        */
    static size_t cache_len = 0;    /* number of primes stored      */
    static size_t cache_cap = 0;    /* allocated capacity of cache  */
    static seq_t last_prime = 2;    /* last prime examined          */

    if (n == 0) return 0;                 /* safety for offset 0          */

    /* already computed? */
    if (n <= cache_len) return cache[n - 1];

    /* generate further primes until we reach the nth term */
    while (cache_len < n) {
        uint64_t nextp64 = next_prime((uint64_t)last_prime);
        seq_t   nextp   = (seq_t)nextp64;

        if (((seq_t)last_prime + nextp) % (seq_t)8 == 0) {
            /* enlarge cache if needed */
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = last_prime;
        }
        last_prime = nextp;  /* advance to the next prime */
    }

    return cache[n - 1];
}
