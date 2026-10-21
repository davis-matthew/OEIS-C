/*
 * A286624  a(n) = (prime(1+n)*prime(n)) + prime(n) + 1.
 * https://oeis.org/A286624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+1) * prime(n) + prime(n) + 1 (offset 1)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

static uint64_t *prime_cache = NULL;
static size_t     prime_cache_cnt = 0;

seq_t a(idx_t n)
{
    if (n == 0)                      /* outside the OEIS offset */
        return (seq_t)0;

    idx_t need = n + 1;              /* need primes up to index n+1 */

    if (prime_cache_cnt < (size_t)need) {
        uint32_t *tmp = NULL;
        size_t    tmpcnt = 0;

        /* obtain the first need primes (1‑based) */
        sieve_first_n_primes(need, &tmp, &tmpcnt);

        /* allocate a new cache of uint64_t primes */
        uint64_t *new_cache = (uint64_t *)malloc(tmpcnt * sizeof(uint64_t));
        if (!new_cache) {
            free(tmp);
            return (seq_t)0;                 /* allocation failed */
        }

        for (size_t i = 0; i < tmpcnt; ++i)
            new_cache[i] = (uint64_t)tmp[i];

        free(tmp);
        free(prime_cache);                  /* discard old cache */
        prime_cache = new_cache;
        prime_cache_cnt = tmpcnt;
    }

    /* prime(n)  is at index n-1 (zero‑based) in the cache   */
    /* prime(n+1) is at index n                                       */
    seq_t p_n   = (seq_t)prime_cache[n - 1];
    seq_t p_np1 = (seq_t)prime_cache[n];

    return p_np1 * p_n + p_n + (seq_t)1;
}
