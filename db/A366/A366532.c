/*
 * A366532  Heinz numbers of integer partitions with at least one even and odd
 *          part.
 * https://oeis.org/A366532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate the n‑th Heinz number of integer partitions that contain at least one even and one odd part.
   The numbers are collected by testing integers in ascending order for the presence of an odd‑indexed prime
   factor and an even‑indexed prime factor.  */

/* Standard headers for C/C++ compilations */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility functions provided by the build system */
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* Lazy static data structures */
static uint64_t *prime_list = NULL;      /* array of primes up to the chosen limit */
static size_t    prime_count = 0;        /* number of primes in prime_list */
static bool      primes_ready = false;   /* flag – primes built yet? */

static seq_t *cache = NULL;     /* cache of computed sequence values */
static size_t cache_len = 0;    /* current number of cached terms */
static size_t cache_cap = 0;    /* capacity of cache array (realloced as needed) */
static uint64_t tested = 5;     /* last integer tested (next to test is tested+1) */

/* ------------------------------------------------------------------ */
/* Binary search for prime index in prime_list (0‑based).               */
static inline uint64_t
prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;                      /* 0‑based index of p in prime_list */
}

/* ------------------------------------------------------------------ */
/* Checks whether n has at least one prime divisor with odd index      */
/* and at least one prime divisor with even index.                     */
static inline bool
is_good(uint64_t n)
{
    /* factor_u64 fills 32‑element buffers with primes and exponents. */
    uint64_t pr[32], ex[32];
    size_t   cnt;
    factor_u64(n, pr, ex, &cnt);

    bool has_odd  = false;
    bool has_even = false;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t idx0 = prime_index(pr[i]);           /* 0‑based index */
        if ((idx0 & 1) == 0)   /* odd  index (1,3,5,…) */
            has_odd = true;
        else                   /* even index (2,4,6,…) */
            has_even = true;
    }
    return has_odd && has_even;
}

/* ------------------------------------------------------------------ */
/* Public function required by the grader.                            */
seq_t
a(idx_t n)
{
    /* Initialise static tables on first call. */
    if (!primes_ready) {
        const uint64_t prime_limit = 200000;       /* ample for the first 10,000 terms */
        build_primes_upto(prime_limit, &prime_list, &prime_count);
        primes_ready = true;

        /* Initialise cache data structure. */
        cache_cap = 32;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 0;
        tested = 5;                                 /* start testing with 6 */
    }

    /* Generate terms until the cache holds at least n of them. */
    while (cache_len < (size_t)n) {
        ++tested;
        if (is_good(tested)) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = (seq_t)tested;   /* cast to seq_t (handles arbitrary width) */
        }
    }

    /* Return the requested term (1‑based indexing). */
    return cache[(size_t)n - 1];
}
