/*
 * A259185  Numbers n such that 1 - sigma(n) + sigma(n)^2 is prime.
 * https://oeis.org/A259185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th positive integer such that * 1 - σ(n) + σ(n)^2
 *            is prime, where σ(n) = sum of all divisors of n
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 1737,
 *            u128 n <= 1737, big any n
 * Helpers:   is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the sequence lazily, caching found values. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;     /* array of sequence values */
static size_t cache_size = 0;   /* number of cached terms */
static size_t cache_cap = 0;    /* current allocated capacity */
static idx_t max_checked = 0;   /* largest n examined so far */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure, terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* Extend the cache until it contains at least 'target' terms. */
static void extend_cache(idx_t target)
{
    while (cache_size < (size_t)target) {
        ++max_checked;                                 /* next integer to test */
        seq_t sigma = sigma_of(max_checked);            /* σ(max_checked) */
        seq_t val = sigma * sigma - sigma + 1;          /* 1 - σ + σ² */
        if (is_prime_big(val)) {
            if (cache_size == cache_cap) ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = (seq_t)max_checked;   /* store the integer */
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0, but return 0 */
    if (cache_size < (size_t)n) extend_cache(n);
    return cache[n - 1];
}
