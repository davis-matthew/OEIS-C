/*
 * A268859  Prime numbers ending in 21.
 * https://oeis.org/A268859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   isPrimeCandidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime numbers ending in 21: a(n) is the n-th prime p with p % 100 == 21. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeCandidate.h"

/* Cached sequence of primes ending in 21. */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* Next k value so that 100*k+21 is the next candidate to test. */
static idx_t next_k = 0;

/* -------------------------------------------------------------------------- */
/*  Helper: primality test for seq_t using trial division.                    */

/* -------------------------------------------------------------------------- */
/*  Helper: grow cache array to at least 'needed' elements.                    */
/* -------------------------------------------------------------------------- */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();      /* Abort on allocation failure. */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* -------------------------------------------------------------------------- */
/*  Ensure that cache contains at least 'n' primes ending in 21.               */
/* -------------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    grow_cache((size_t)n);

    if (next_k == 0) next_k = 4; /* 4*100+21 = 421, the first such prime */

    while (cache_len < (size_t)n) {
        seq_t cand = (seq_t)100 * (seq_t)next_k + (seq_t)21;
        if (isPrimeCandidate(cand)) cache[cache_len++] = cand;
        ++next_k;
    }
}

/* -------------------------------------------------------------------------- */
/*  Sequence function.                                                        */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined for n < 1, return 0 as a safe value. */

    ensure_cache(n);

    return cache[n - 1];
}
