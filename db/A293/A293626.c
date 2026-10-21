/*
 * A293626  Numbers of the form (2^(2p) + 1)/5, where p is a prime > 5.
 * https://oeis.org/A293626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(2p) + 1)/5 where p is the nth prime >5 (OEIS A293626)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   next_prime, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/next_prime.h"

/* cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* cache of primes >5 */
static uint64_t *prime_cache = NULL;
static size_t prime_cap = 0;
static size_t prime_size = 0;

/* ensure that we have at least n primes >5 in prime_cache */
static void ensure_primes(size_t n)
{
    if (prime_size >= n)
        return;

    /* initialise with first prime >5 */
    if (prime_size == 0) {
        prime_cap = 16;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_cache[0] = 7;              /* first prime >5 */
        prime_size = 1;
    }

    while (prime_size < n) {
        uint64_t last = prime_cache[prime_size - 1];
        uint64_t next = next_prime(last);
        if (prime_size == prime_cap) {
            prime_cap *= 2;
            prime_cache = (uint64_t *)realloc(prime_cache,
                                              prime_cap * sizeof(uint64_t));
        }
        prime_cache[prime_size++] = next;
    }
}

/* ensure cache big enough to hold n terms, compute missing terms */
static void ensure_cache(size_t n)
{
    if (cache_size >= n)
        return;

    /* grow cache if needed */
    if (cache_size == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* ensure primes */
    ensure_primes(n);

    /* compute missing terms */
    for (size_t i = cache_size; i < n; ++i) {
        uint64_t p = prime_cache[i];                    /* prime index i+1 */
        seq_t pow = pow_seq((seq_t)2, (idx_t)(2U * (idx_t)p));
        seq_t val = (pow + (seq_t)1) / (seq_t)5;
        cache[i] = val;
    }
    cache_size = n;
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (n == 0)      /* offset begins at 1; behaviour for n<1 is undefined */
        return (seq_t)0;

    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
