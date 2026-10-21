/*
 * A283809  Squarefree numbers k such that 6*k - 1 and 6*k + 1 are twin
 *          primes.
 * https://oeis.org/A283809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 2194, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A283809: squarefree k such that 6*k–1 and 6*k+1 are twin primes. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper functions provided by the build system */
#include "../utils/is_prime.h"       /* bool is_prime(idx_t x) */
#include "../utils/is_squarefree.h" /* bool is_squarefree(seq_t x) */

/* Static state used to cache sequence values independently of call order */
static seq_t *seq_cache   = NULL; /* array of term values          */
static size_t cache_size  = 0;    /* number of terms already cached*/
static size_t cache_cap   = 0;    /* allocated capacity           */
static idx_t   search_k   = 1;    /* next k to be examined        */

/* Ensure the cache has room for at least new_cap elements */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t new_size = cache_cap ? cache_cap << 1 : 128;
    if (new_size < new_cap) new_size = new_cap;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
    if (!tmp) abort();              /* out of memory: terminate */
    seq_cache = tmp;
    cache_cap = new_size;
}

/* Append a value to the cache */
static void cache_append(seq_t val)
{
    ensure_cache_capacity(cache_size + 1);
    seq_cache[cache_size++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined, but safe */

    /* Produce terms until we have enough cached */
    while (cache_size < n)
    {
        /* Check whether 6*k-1 and 6*k+1 are primes and k is squarefree */
        idx_t low  = 6 * search_k - 1;
        idx_t high = 6 * search_k + 1;

        if (is_prime(low) && is_prime(high) && is_squarefree((seq_t)search_k))
        {
            cache_append((seq_t)search_k);
        }
        ++search_k;                       /* next k to test */
    }

    return seq_cache[n - 1];
}
