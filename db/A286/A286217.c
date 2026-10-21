/*
 * A286217  Product of the n-th sexy prime triple.
 * https://oeis.org/A286217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of the n‑th sexy prime triple a(n) = p * (p+6) *
 *            (p+12), where p, p+6, p+12 are primes and p+18 is not prime
 * Range:     u8 none, u16 n <= 3, u32 n <= 32, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x); */

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_size = 0;        /* number of terms stored in cache      */
static size_t cache_cap  = 0;        /* current capacity of the cache        */
static uint64_t last_checked = 7;    /* smallest p not yet examined          */

/* Ensure the cache can hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort(); /* memory exhausted */
    cache = tmp;
    cache_cap = new_cap;
}

/* Find the next sexy prime triple and append its product to 'cache'. */
static void find_next(void)
{
    for (uint64_t p = last_checked;; ++p) {
        if (is_prime(p) && is_prime(p + 6) && is_prime(p + 12) && !is_prime(p + 18)) {
            seq_t prod = (seq_t)p;
            prod *= (seq_t)(p + 6);
            prod *= (seq_t)(p + 12);
            grow_cache(cache_size + 1);
            cache[cache_size++] = prod;
            last_checked = p + 1;   /* start next search after this p */
            break;
        }
    }
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1, n==0 is undefined */
    while (cache_size < n) find_next();
    return cache[n - 1];
}
