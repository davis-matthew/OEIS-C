/*
 * A307898  Expansion of 1/(1 - x * Sum_{k>=1} prime(k)*x^k).
 * https://oeis.org/A307898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, for n>=2: a(n)=∑_{k=1}^{n-1} prime(k)*a(n-1-k)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Get the k-th prime (k>=1).  The helper is provided by the build system. */
#include "../utils/get_prime.h"

/* ---------- static cache for the sequence values ---------- */
static seq_t *seq_cache   = NULL;  /* array of computed a(n) values   */
static size_t cache_cap   = 0;     /* current allocated capacity      */
static idx_t  cache_max   = 0;     /* largest n for which a(n) is stored */

/* Initialise the cache with a(0)=1, a(1)=0 (at least two entries). */
static void init_cache(idx_t n)
{
    if (seq_cache != NULL)          /* already initialised */
        return;

    /* capacity must be big enough for at least the requested n                */
    size_t cap = 2;
    while (cap <= (size_t)n)
        cap <<= 1;

    seq_cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!seq_cache)                /* allocation failed – leave empty */
        return;

    seq_cache[0] = (seq_t)1;       /* a(0) */
    seq_cache[1] = (seq_t)0;       /* a(1) */
    cache_cap = cap;
    cache_max = 1;
}

/* Expand the cache so that indices 0..n can be stored. */
static void expand_cache(idx_t n)
{
    if (n < (idx_t)cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)        /* realloc failed – leave old cache unchanged        */
        return;
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)                 /* base case a(0) */
        return (seq_t)1;
    if (n == 1)                 /* base case a(1) */
        return (seq_t)0;

    init_cache(n);              /* ensure cache can hold at least a(0) and a(1) */
    expand_cache(n);            /* make room for any higher indices */

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i - 1; ++k) {
            uint64_t prime_k = get_prime(k);        /* k‑th prime (k>=1) */
            sum += (seq_t)prime_k * seq_cache[i - 1 - k];
        }
        seq_cache[i] = sum;
    }
    cache_max = n;
    return seq_cache[n];
}
