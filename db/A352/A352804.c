/*
 * A352804  a(n) = A028876(n)/2; numbers k such that 4*k^2 - 5 is prime.
 * https://oeis.org/A352804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth integer k ≥ 2 such that 4*k*k - 5 is prime
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 4902, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the k values themselves, starting from 2. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* array of found k's */
static size_t cache_len   = 0;        /* number stored */
static size_t cache_cap   = 0;        /* allocated capacity */
static idx_t  next_k      = 2;        /* next integer to test */

/* Ensure the cache has enough space. */
static void grow_cache(void) {
    size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a new value to the cache. */
static void add_to_cache(idx_t k) {
    if (cache_len == cache_cap) grow_cache();
    cache[cache_len++] = (seq_t)k;
}

/* Check whether 4*k^2 - 5 is prime. */
static bool is_k_prime(idx_t k) {
    /* Compute the value in 128‑bit to avoid overflow. */
    __uint128_t val128 = (__uint128_t)4 * k * k - 5;

    /* Use the appropriate prime test depending on the size of seq_t. */
    if (sizeof(seq_t) <= 8) {
        seq_t v = (seq_t)val128;          /* truncation is safe for the tested range */
        return is_prime_small(v);
    } else {
        seq_t v = (seq_t)val128;          /* seq_t is at least 128 bits or arbitrary precision */
        return is_prime_big(v);
    }
}

/* Pre‑compute terms up to index n (1‑based). */
static void precompute_until(idx_t n) {
    while (cache_len < n) {
        if (is_k_prime(next_k))
            add_to_cache(next_k);
        ++next_k;
    }
}

/* Public routine.  n is 1‑based; return 0 for n == 0 (out of range). */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    precompute_until(n);
    return cache[(size_t)(n - 1)];
}
