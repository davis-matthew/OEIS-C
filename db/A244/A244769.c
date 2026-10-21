/*
 * A244769  Prime numbers ending in the prime number 43.
 * https://oeis.org/A244769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 159, u32 n <= 115, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the primes of the form 100*k + 43.
   The nth term is the nth such prime (a(1)=43).  */
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_k = 0;

/* Append value to cache, expanding if needed */
static void append(seq_t val) {
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;  /* allocation failure, ignore for simplicity */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1; undefined below */

    /* Generate primes until we have the nth term */
    while (cache_len < n) {
        /* candidate = 100*k + 43 */
        seq_t candidate = (seq_t)(100 * next_k + 43);
        next_k++;

        /* Test primality */
        if (is_prime_big(candidate)) {
            append(candidate);
        }
    }
    return cache[n - 1];
}
