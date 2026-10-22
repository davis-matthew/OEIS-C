/*
 * A301870  Primes of form 4*k^k - 3.
 * https://oeis.org/A301870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*A301521(n)^A301521(n) - 3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"

/* Cached results of the sequence */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_capacity = 0;
static idx_t next_k = 2;  /* Next k to test (starting from 2) */

/* Ensure the cache has room for at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (cache_capacity < need) {
        size_t new_capacity = cache_capacity ? cache_capacity * 2 : 16;
        while (new_capacity < need)
            new_capacity *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory – abort */
            abort();
        }
        cache = tmp;
        cache_capacity = new_capacity;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* Undefined; return 0 as a placeholder */

    /* Grow cache until we have at least n terms */
    while (cache_size < n) {
        /* Compute 4 * k^k - 3 */
        seq_t k_seq = (seq_t)next_k;
        seq_t pow_val = big_pow(k_seq, k_seq);
        seq_t candidate = pow_val * ((seq_t)4);
        candidate = candidate - ((seq_t)3);

        if (is_prime_big(candidate)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = candidate;
        }

        ++next_k;
    }

    return cache[n - 1];
}
