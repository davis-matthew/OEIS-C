/*
 * A244768  Prime numbers ending in the prime number 37.
 * https://oeis.org/A244768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes of the form 100*k + 37 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

static seq_t *prime_cache = NULL;   /* stored primes of the A.P. */
static size_t cache_cap = 0;        /* current capacity of prime_cache */
static idx_t  cache_cnt = 0;        /* number of primes stored */
static idx_t  next_k    = 0;        /* k value for the next candidate */

/* Ensure the cache can hold at least 'need' elements */
static void ensure_cache_cap(size_t need) {
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t*)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failed */
    prime_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;  /* below offset, value irrelevant */

    while (cache_cnt < n) {
        idx_t cand = 100 * next_k + 37;   /* candidate of the form 100*k+37 */
        if (is_prime_small(cand)) {
            ensure_cache_cap(cache_cnt + 1);
            prime_cache[cache_cnt] = (seq_t)cand;
            ++cache_cnt;
        }
        ++next_k;
    }
    return prime_cache[n - 1];
}
