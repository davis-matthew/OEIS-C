/*
 * A270189  Numbers n for which (prime(n+1)-prime(n)) is not a multiple of
 *          three.
 * https://oeis.org/A270189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that prime(k+1)-prime(k) is not a
 *            multiple of 3. a(1) = 1, a(2) = 2, a(3) = 3, …
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                  /* malloc, free, realloc */
#include "../utils/build_primes_upto.h"  /* build_primes_upto */

seq_t a(idx_t n) {
    /* static storage for memoisation */
    static uint64_t *index_cache = NULL;   /* cache of qualifying indices (1‑based) */
    static size_t cache_len  = 0;           /* number of elements stored */
    static size_t cache_cap  = 0;           /* allocated capacity of cache */

    static uint64_t *primes  = NULL;       /* array of primes */
    static size_t   prime_cnt  = 0;        /* number of primes in primes */
    static idx_t    prime_lim  = 0;        /* current sieving limit */
    static size_t   processed  = 0;        /* number of gaps examined */

    if (n == 0)   /* the sequence has offset 1 – this case is unused, but we
                     avoid undefined behaviour if called with n==0. */
        return (seq_t)0;

    /* grow the cache until it contains at least n elements */
    while (cache_len < (size_t)n) {
        /* Ensure we have enough primes to inspect the next gap. */
        while (processed + 1 >= prime_cnt) {  /* need at least two primes */
            idx_t new_lim = (prime_lim == 0) ? 100 : prime_lim * 2;
            uint64_t *new_primes = NULL;      /* new primes array */
            size_t   new_cnt   = 0;
            /* free old primes before allocating new ones */
            if (primes) free(primes);
            build_primes_upto(new_lim, &new_primes, &new_cnt);
            primes   = new_primes;
            prime_cnt = new_cnt;
            prime_lim = new_lim;
        }

        /* examine the gap between primes[processed] and primes[processed+1] */
        uint64_t diff = primes[processed + 1] - primes[processed];
        if (diff % 3u != 0u) {                 /* gap not a multiple of 3 */
            /* enlarge cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                if (new_cap < cache_len + 1) new_cap = cache_len + 1;
                uint64_t *tmp = (uint64_t *)realloc(index_cache,
                                                     new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);             /* allocation failure */
                index_cache = tmp;
                cache_cap   = new_cap;
            }
            /* store the 1‑based index of the first prime in this gap */
            index_cache[cache_len++] = (uint64_t)(processed + 1);
        }
        processed++;                           /* move to next gap */
    }

    /* return the requested term, cast to seq_t */
    return (seq_t)index_cache[n - 1];
}
