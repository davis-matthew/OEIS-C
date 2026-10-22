/*
 * A313764  Coordination sequence Gal.3.61.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   alloc_seq, realloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A313764 – Gal.3.61.1
    Recurrence:  a(n) = -a(n-1) + a(n-3) + a(n-4) + a(n-6) + a(n-7)
                     - a(n-9) - a(n-10)          for n > 10
    Initial values:  a(0)=1, a(1)=5, a(2)=10, a(3)=15, a(4)=21, a(5)=24,
                    a(6)=32, a(7)=34, a(8)=41, a(9)=45, a(10)=52
*/

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/realloc_seq.h"

/* Stored initial values */
static const seq_t base_vals[11] = {
    (seq_t)1,  (seq_t)5,  (seq_t)10, (seq_t)15, (seq_t)21,
    (seq_t)24, (seq_t)32, (seq_t)34, (seq_t)41, (seq_t)45,
    (seq_t)52
};

/* Cache for computed terms */
static seq_t  *cache     = NULL;   /* pointer to cached values          */
static size_t  cache_cap = 0;      /* allocated capacity of cache       */
static idx_t   cache_len = 0;      /* number of values currently stored */

seq_t a(idx_t n)
{
    /* Handle the known initial values directly */
    if (n <= 10u) {
        return base_vals[(size_t)n];
    }

    /* Lazy initialization of the cache */
    if (cache == NULL) {
        /* Allocate a modest initial capacity */
        cache_cap = 64;
        cache     = alloc_seq(cache_cap);
        if (cache == NULL) {
            exit(EXIT_FAILURE); /* Allocation failure */
        }

        /* Copy the first 11 terms into the cache */
        for (size_t i = 0; i <= 10; ++i) {
            cache[(size_t)i] = base_vals[(size_t)i];
        }
        cache_len = 11u;
    }

    /* Grow the cache capacity if requested index exceeds it */
    if ((size_t)(n + 1u) > cache_len) {
        /* Ensure the internal array can hold up to n+1 elements */
        if ((size_t)(n + 1u) > cache_cap) {
            size_t new_cap = cache_cap;
            while ((size_t)(n + 1u) > new_cap) {
                new_cap <<= 1;
            }
            seq_t *tmp = realloc_seq(cache, new_cap);
            if (tmp == NULL) {
                exit(EXIT_FAILURE); /* Allocation failure */
            }
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* Compute the missing terms up to n, using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t pos = cache[(size_t)i - 3u] + cache[(size_t)i - 4u]
                      + cache[(size_t)i - 6u] + cache[(size_t)i - 7u];
            seq_t neg = cache[(size_t)i - 1u] + cache[(size_t)i - 9u]
                      + cache[(size_t)i -10u];
            cache[(size_t)i] = pos - neg;
        }
        cache_len = n + 1u;
    }

    return cache[(size_t)n];
}
