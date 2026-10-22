/*
 * A361484  Primes p such that p + 512 is also prime.
 * https://oeis.org/A361484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   The function returns the n‑th prime p such that p + 512 is also prime.
   The sequence is indexed as in OEIS: a(1) = 11, a(2) = 29, … */
   
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* static state ---------------------------------------------------------- */
static seq_t *cache      = NULL;   /* array of found numbers              */
static size_t cache_size  = 0;     /* number of elements in cache        */
static size_t cache_cap   = 0;     /* current capacity of cache          */
static uint64_t next_candidate = 2;/* next integer to test for primality */

/* expand the cache array if necessary ----------------------------------- */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 4;
    /* realloc returns void*, cast explicitly for C++ */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;               /* out of memory – leave cache empty */
    cache = tmp;
    cache_cap = new_cap;
}

/* populate cache until it contains at least n elements ------------------ */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n) return;

    /* iterate over successive integers, looking for primes p such that
       p + 512 is also prime.  We keep the generation state in
       next_candidate so the search continues where it left off.          */
    for ( ; cache_size < (size_t)n ; ++next_candidate )
    {
        /* check that next_candidate is prime and that the shifted value
           is prime as well.  is_prime works on idx_t (uint64_t).     */
        uint64_t p = next_candidate;
        uint64_t q = p + 512;
        if (is_prime(p) && is_prime(q))
        {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)p;   /* cast to seq_t works in C and C++ */
        }
    }
}

/* the required sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0, safe default */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
