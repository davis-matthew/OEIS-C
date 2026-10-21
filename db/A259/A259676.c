/*
 * A259676  Heptagonal numbers (A000566) that are semiprimes (A001358).
 * https://oeis.org/A259676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = H(n) where H(n) = n*(5*n-3)/2 is a heptagonal number and
 *            a semiprime with exactly two (not necessarily distinct) prime
 *            factors
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Primality helpers provided by the utils library */
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

/* Count prime factors (with multiplicity) – a semiprime has exactly two. */
#include "../utils/count_prime_factors.h"

/* Helper to test if a value is a semiprime. */
static bool is_semiprime(seq_t x)
{
    /* 0,1,2,3 cannot be semiprime */
    if (x < 4) return false;
    return count_prime_factors(x) == 2;
}

/* Return the n-th element of A259676 (1‑based). */
seq_t a(idx_t n)
{
    /* Cache of previously found values for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t last_k = 0;   /* largest k examined so far */

    /* Grow the cache until it contains at least n values. */
    while (cache_size < n) {
        ++last_k;
        /* use a 64‑bit intermediate to avoid overflow for small seq_t's */
        uint64_t k = (uint64_t)last_k;
        uint64_t H64 = k * (5 * k - 3) / 2;
        seq_t H = (seq_t)H64;    /* cast to the actual seq_t type */

        if (is_semiprime(H)) {
            /* Ensure capacity. */
            if (cache_size + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                if (new_cap < cache_size + 1)
                    new_cap = cache_size + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – abort; in contest harness this
                       will not happen, but we keep it for safety. */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = H;
        }
    }

    return cache[n - 1];   /* a(1) is the first term stored. */
}
