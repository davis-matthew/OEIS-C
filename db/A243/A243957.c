/*
 * A243957  Primes of the form 2*n^2+66*n+31.
 * https://oeis.org/A243957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 21, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes of the form 2*n^2 + 66*n + 31, offset 1 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Cache of primes that have been found so far. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;   /* number of primes stored */
static size_t cache_cap = 0;   /* current allocation size */
static idx_t  next_k    = 1;   /* next k to test */

static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    seq_t *tmp = seq_realloc(cache, cache_len, new_cap);
    if (!tmp) abort();            /* out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th prime of the form 2*k^2+66*k+31  (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n<1, return 0 as placeholder */

    /* Extend the cache until we have at least n primes. */
    while (cache_len < (size_t)n) {
        seq_t k   = (seq_t)next_k;
        seq_t val = ((seq_t)2) * k * k + ((seq_t)66) * k + (seq_t)31;

        if (is_prime_big(val)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = val;
        }
        next_k++;
    }

    return cache[(size_t)n - 1];   /* zero‑based index in the cache */
}
