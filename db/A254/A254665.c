/*
 * A254665  Primes prime(n) such that prime(n) + 7*n is also prime.
 * https://oeis.org/A254665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254665: primes p such that p + 7*n is prime where n is the
   index of p among the primes (1‑based).  Example terms:
   a(1)=3, a(2)=71, a(3)=79, …  */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

/* Cache of already found terms.  cache[i] holds a(i+1). */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;

/* Current state of prime generation.  prime_index counts primes seen,
   and last_prime holds the last generated prime.  The next prime is
   obtained by next_prime(last_prime). */
static idx_t  prime_index = 0;     /* 1‑based index of the last prime */
static uint64_t last_prime = 1;    /* start < 2 so next_prime(1)=2 */

/* Ensure that cache has room for at least one more element. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache     = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n is 1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1 */

    /* Return cached value if available. */
    if (n <= cache_cnt) return cache[n - 1];

    /* Initialise cache on first use. */
    if (!cache) {
        cache_cap = 64;
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cnt = 0;
    }

    /* Generate primes until we have at least n terms in the sequence. */
    while (cache_cnt < (size_t)n) {
        /* Produce the next prime number. */
        last_prime = next_prime(last_prime);
        ++prime_index;

        /* Compute p + 7 * n where n is the prime's index. */
        seq_t sum = (seq_t)last_prime + ((seq_t)7 * (seq_t)prime_index);

        /* If the sum is also prime, record the prime. */
        if (is_prime_big(sum)) {
            if (cache_cnt >= cache_cap) grow_cache();
            cache[cache_cnt++] = (seq_t)last_prime;
        }
    }

    return cache[n - 1];
}
