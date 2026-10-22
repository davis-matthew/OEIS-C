/*
 * A363049  Even numbers k having fewer prime factors, counted with
 *          multiplicity, than k-1.
 * https://oeis.org/A363049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth even integer k such that A001222(k) <
 *            A001222(k-1) (fewer prime factors, counted with multiplicity)
 * Range:     u8 n <= 8, u16 n <= 4037, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/bigomega.h"   /* seq_t bigomega(idx_t) */

seq_t a(idx_t n)
{
    /* Static state: cache of found terms (stored as uint64_t to avoid
       constructing arbitrary‑precision objects during allocation).      */
    static uint64_t *cache   = NULL;   /* array of found even k          */
    static size_t  cache_len = 0;      /* number of cached terms         */
    static size_t  cache_cap = 0;      /* allocated capacity             */
    static uint64_t next_even = 2;     /* next even number to test       */
    static int      initialized = 0;   /* guard against concurrent use   */

    /* Ensure thread safety in C++ (static local initialization) */
    if (!initialized) {
        /* zero initial state */
        cache = NULL;
        cache_len = 0;
        cache_cap = 0;
        next_even = 2;
        initialized = 1;
    }

    /* Grow cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        seq_t omega_k  = bigomega(next_even);
        seq_t omega_k1 = bigomega(next_even - 1);

        if (omega_k < omega_k1) {
            /* resize if needed */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                uint64_t *new_cache = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
                /* In case realloc fails we ignore the failure: the program will
                   just keep previous cache and try again next request. */
                if (new_cache == NULL) return 0;  /* cannot allocate; return 0 as sentinel */
                cache     = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_even;
        }
        next_even += 2;   /* only even numbers need be tested */
    }

    return (seq_t)cache[n - 1];
}
