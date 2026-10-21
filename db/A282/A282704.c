/*
 * A282704  (Twice product of first n primes) - 1.
 * https://oeis.org/A282704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Π_{k=0}^{n-1} primes(k) - 1 , a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ build – use std::vector for cached values and primes */
    static std::vector<seq_t> cache;       /* a(0)..a(cache.size()-1) computed */
    static std::vector<uint32_t> primes;   /* first primes[0]..primes[primes.size()-1] */

    /* Ensure we have the first n primes */
    if (primes.size() < (size_t)n) {
        uint32_t *tmp = NULL;
        size_t tmpcnt = 0;
        sieve_first_n_primes(n, &tmp, &tmpcnt);   /* tmpcnt == n */
        primes.assign(tmp, tmp + tmpcnt);
        free(tmp);
    }

    /* Compute missing terms up to n */
    while (cache.size() <= (size_t)n) {
        if (cache.empty()) {
            cache.push_back((seq_t)1);            /* a(0) */
        } else {
            seq_t temp = cache.back() + (seq_t)1;
            temp = temp * (seq_t)primes[cache.size() - 1];
            cache.push_back(temp - (seq_t)1);
        }
    }
    return cache[(size_t)n];
#else
    /* C build – use malloc/realloc for cached values and primes */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated capacity */
    static size_t cache_len  = 0;   /* number of computed terms */

    static uint32_t *primes  = NULL;
    static size_t     primes_cnt = 0;

    /* Ensure cache can hold n+1 entries */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;      /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Ensure we have the first n primes */
    if (primes_cnt < (size_t)n) {
        uint32_t *tmp = NULL;
        size_t tmpcnt = 0;
        sieve_first_n_primes(n, &tmp, &tmpcnt);   /* tmpcnt == n */
        if (primes) free(primes);
        primes = tmp;
        primes_cnt = tmpcnt;
    }

    /* Compute missing terms up to n */
    while (cache_len <= (size_t)n) {
        if (cache_len == 0) {
            cache[0] = (seq_t)1;                /* a(0) = 1 */
        } else {
            seq_t temp = cache[cache_len - 1] + (seq_t)1;
            temp = temp * (seq_t)primes[cache_len - 1];
            cache[cache_len] = temp - (seq_t)1;
        }
        cache_len++;
    }

    return cache[(size_t)n];
#endif
}
