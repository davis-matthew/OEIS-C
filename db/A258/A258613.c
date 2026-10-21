/*
 * A258613  Numbers m that are coprime to the largest square <= m, cf.
 *          A048760.
 * https://oeis.org/A258613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer m such that gcd(m, ⌊√m⌋) = 1
 *            (OEIS A258613). The implementation uses a simple dynamic cache
 *            that grows as needed. It is valid for any seq_t type that
 *            supports the required arithmetic and the gcd and floor_sqrt
 *            utilities
 * Range:     u8 n <= 37, u16 n <= 7395, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uintX_t types */
#include <stdbool.h>     /* bool */
#include <stdlib.h>      /* malloc, realloc, free */

#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */

/* Static cache for previously computed terms. */
static seq_t *cache      = NULL;  /* array of computed terms */
static size_t cache_size = 0;     /* number of terms currently stored */
static size_t cache_cap  = 0;     /* allocated capacity (in elements) */

/* Ensure that the cache has at least `need` elements.  The cache is
   expanded by doubling its capacity until it meets the requirement. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_ptr == NULL) {
        /* Allocation failed – abort the program. */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Return the nth term of A258613;  n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;    /* undefined for n < 1, but shall never occur */

    /* Convert the index to a size_t for array operations. */
    size_t needed = (size_t)n;

    /* Grow the cache if necessary. */
    if (needed > cache_size) {
        ensure_cache_capacity(needed);

        seq_t m = (cache_size > 0) ? cache[cache_size - 1] : (seq_t)0;
        for (size_t i = cache_size; i < needed; ++i) {
            /* Find the next integer m such that gcd(m, ⌊√m⌋) == 1. */
            do {
                ++m;
            } while (gcd(m, floor_sqrt(m)) != (seq_t)1);
            cache[i] = m;
        }
        cache_size = needed;
    }
    return cache[n - 1];   /* convert to 0‑based index */
}
