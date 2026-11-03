/*
 * A348421  Primes p == 3 (mod 4) such that (p+3)/2 is not prime.
 * https://oeis.org/A348421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2310, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9588 (2026-11-03)
 * Helpers:   is_prime_seq, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A348421: primes p ≡ 3 (mod 4) such that (p+3)/2 is composite.
    The function searches progressively for such primes and caches the
    results, so calls may be in any order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

static seq_t *term_cache = NULL;   /* cached a(n) values                */
static size_t cache_len   = 0;     /* number of cached terms             */
static size_t cache_cap   = 0;     /* allocated cache capacity           */
static uint64_t last_start = 3;    /* last prime examined                */

seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        uint64_t p;

        /* Find the next prime p ≡ 3 (mod 4) with composite (p+3)/2. */
        for (;;)
        {
            p = next_prime(last_start + 1);
            last_start = p; /* advance for next search */

            if ((p & 3ULL) != 3ULL)                     /* not 3 mod 4 */
                continue;

            uint64_t mid = (p + 3ULL) >> 1;            /* (p+3)/2      */
            if (!is_prime_seq(mid))                  /* composite mid */
                break;                                 /* found a term */
        }

        /* Grow the cache if necessary. */
        if (cache_len == cache_cap)
        {
            cache_cap = cache_cap ? cache_cap << 1 : 16;
            seq_t *tmp = mem_resize(seq_t, term_cache, cache_cap);
            if (!tmp) abort();                         /* out of memory */
            term_cache = tmp;
        }

        term_cache[cache_len++] = (seq_t)p;
    }

    return term_cache[n - 1];
}
