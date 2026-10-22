/*
 * A381004  Primes ending in 777.
 * https://oeis.org/A381004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1000*A102343(n)+777
 * Range:     u8 none, u16 n <= 12, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102343(n) is the n-th prime congruent to 3 (mod 1000). */
/* This function iterates over candidates 1000*k+777 and tests primality. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"   /* provides bool isPrimeGeneric(seq_t) */

static seq_t *cache = NULL;          /* cached primes of the form 1000*k+777 */
static size_t cache_len = 0;          /* number of cached primes */
static size_t cache_cap = 0;          /* allocated capacity of cache   */
static idx_t  next_k = 1;             /* k of the next candidate to test */

/* Make sure the cache can hold at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* Out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th prime ending in 777. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* Offset is 1; for n=0 return 0 */

    /* Ensure the cache holds the first n primes. */
    while (cache_len < (size_t)n) {
        seq_t cand = (seq_t)next_k * (seq_t)1000 + (seq_t)777;
        if (isPrimeGeneric(cand)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = cand;
        }
        ++next_k;
    }
    return cache[n - 1];
}
