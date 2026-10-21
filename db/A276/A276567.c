/*
 * A276567  Odd squares not of the form p + 2^k with p prime.
 * https://oeis.org/A276567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 433, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A276567: odd squares not representable as p + 2^k (p prime). */
/* The function is lazy-fully memoised; it works for any seq_t/idx_t
   combination supported by the build, and callers may request terms in
   any order. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

/* Memoisation storage */
static seq_t *cache = NULL;          /* array of computed terms          */
static size_t cache_size = 0;        /* number of stored terms           */
static size_t cache_cap  = 0;        /* allocated capacity of cache      */

/* The next odd integer to check for its square. */
static idx_t current_m = 1;          /* starts at 1 (odd) */

/* Test whether an odd square s can be written as p + 2^k with p prime. */
static bool is_representable(seq_t s)
{
    for (seq_t pow = (seq_t)1; pow < s; pow <<= 1U) {
        seq_t p = s - pow;
        if (p < (seq_t)2) {
            continue;           /* primes start at 2 */
        }
        if (isPrimeGeneric(p)) {
            return true;        /* found a representation */
        }
    }
    return false;                /* no representation */
}

/* Ensure that cache contains at least n terms.  If n is larger than the
   current cache size, continue generating odd squares until enough
   non‑representable ones are stored. */
static void ensure_cache(idx_t n)
{
    if (!cache) {                         /* first call: allocate storage */
        cache_cap   = 16;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size  = 0;
    }

    while (cache_size < n) {
        seq_t sq = (seq_t)current_m * (seq_t)current_m; /* m² */

        if (!is_representable(sq)) {
            /* Grow the cache if necessary */
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache,
                                         cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = sq;
        }

        current_m += 2U;                    /* next odd integer */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined for index 0, but return 0 safely */
    }

    /* Fill the cache up to the requested index */
    ensure_cache(n);

    return cache[n - 1];
}
