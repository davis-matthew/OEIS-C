/*
 * A350460  Positive integers k such that if p is the next prime > k then p -
 *          k is prime.
 * https://oeis.org/A350460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A350460: positive integers k such that the next prime > k
   has a prime difference p - k.  The function builds the sequence
   incrementally and memoises the terms.  A small prime test for the
   difference is performed by trial division on a 64‑bit integer. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

/* Small helper: primality test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Cached sequence values.  The cache is extended only as needed. */
static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_len = 0;          /* number of terms currently stored */
static size_t cache_cap = 0;          /* allocated capacity of the cache   */
static uint64_t next_candidate = 1;   /* next k to examine (positive integer) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* a(0) is undefined; harmless return */

    /* Extend the cache until it contains at least n elements. */
    while (cache_len < n)
    {
        uint64_t k = next_candidate;           /* current candidate k  */
        uint64_t p = next_prime(k);            /* next prime after k   */
        uint64_t diff = p - k;                 /* difference p - k      */

        /* Test whether diff is prime. */
        if (is_prime_uint64(diff))
        {
            /* Grow the cache if required. */
            if (cache_len == cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();                /* allocation failure → terminate */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)k;       /* store the found term */
        }
        ++next_candidate;
    }

    return cache[n - 1];   /* zero‑based indexing inside the cache */
}
