/*
 * A387775  a(1) = 2; for n > 1, a(n) = a(n-1)*prime(n) if a(n-1) <= prime(n),
 *          otherwise a(n) = a(n-1) mod prime(n).
 * https://oeis.org/A387775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; for n > 1, a(n) = a(n-1) * prime(n) if a(n-1) <=
 *            prime(n), a(n) = a(n-1) % prime(n) otherwise. Off‑set: 1 (a(1)
 *            is the first listed term)
 * Range:     u8 n <= 8, u16 n <= 64, u32 n <= 6692, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/get_prime.h"

/* Cache of computed sequence values; 1‑based indexing. */
static seq_t *cache = NULL;       /* cache[i] = a(i) for i>=1 */
static size_t cache_cap = 0;      /* allocated size (including index 0) */
static size_t cache_len = 0;      /* largest n that has been computed */

/* Ensure that the cache can hold indices up to `needed` (inclusive). */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Main function implementing the recurrence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* Initialise on first call. */
    if (!cache) {
        ensure_cache_capacity(32);   /* arbitrary start size */
        cache[1] = (seq_t)2;         /* a(1) = 2 */
        cache_len = 1;
    }

    /* Ensure the cache is large enough for index n. */
    ensure_cache_capacity((size_t)n + 1);

    /* Compute any missing terms up to n. */
    while (cache_len < (size_t)n) {
        size_t idx = cache_len + 1;                     /* next index (1‑based) */
        seq_t prev = cache[cache_len];
        seq_t prime = (seq_t)get_prime((idx_t)idx);     /* the idx‑th prime */
        if (prev <= prime)
            cache[idx] = prev * prime;
        else
            cache[idx] = prev % prime;
        cache_len = idx;
    }

    return cache[(size_t)n];
}
