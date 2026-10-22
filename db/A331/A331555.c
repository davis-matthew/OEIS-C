/*
 * A331555  Prime numbers p_k such that p_k == 1 (mod 10) and p_(k+1) == 3
 *          (mod 10).
 * https://oeis.org/A331555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p_k satisfying p_k ≡ 1 (mod 10) and the *
 *            immediate next prime p_{k+1} ≡ 3 (mod 10). The sequence is built
 *            * by generating consecutive primes and collecting those that
 *            meet the * condition. The function works for any build of seq_t
 *            and idx_t * and supports out‑of‑order calls...
 * Range:     u8 n <= 6, u16 n <= 557, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/next_prime.h"

/* Static cache of already computed terms */
static seq_t *cache = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;

/* The last prime that was examined as the starting prime of the next
 * pair.  Initially set to 2 so that the first prime produced is 3. */
static uint64_t last_prime = 2;

/* Small helper to grow the cache when needed. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *new_ptr = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (cache) {
        memcpy(new_ptr, cache, cache_cnt * sizeof(seq_t));
        free(cache);
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Append a new value to the cache. */
static void append_cache(seq_t val)
{
    if (cache_cnt >= cache_cap)
        grow_cache();
    cache[cache_cnt++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Convert idx_t to seq_t when indexing the cache. */
    if (n == 0) return 0;           /* outside the sequence; harmless */
    /* Return from cache if we already have it. */
    if ((size_t)n <= cache_cnt)
        return cache[(size_t)n - 1];

    /* Generate primes and find pairs until we have the n-th term. */
    while (cache_cnt < (size_t)n) {
        uint64_t p = next_prime(last_prime);   /* current prime */
        uint64_t q = next_prime(p);            /* immediate next prime */

        if ((p % 10) == 1 && (q % 10) == 3)
            append_cache((seq_t)p);

        last_prime = p;  /* move forward for the next pair test */
    }

    return cache[(size_t)n - 1];
}
