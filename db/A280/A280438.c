/*
 * A280438  Number of n X 6 0..1 arrays with no element equal to more than one
 *          of its horizontal and vertical neighbors, with the exception of
 *          exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6) for
 *            n>7, with initial values a(1)=10, a(2)=68, a(3)=136, a(4)=232,
 *            a(5)=360, a(6)=548, a(7)=834
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 38, u64 n <= 83, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Memoisation cache. Index 0 is unused; cache[n] holds a(n). */
static seq_t *cache      = NULL;   /* pointer to allocated array of seq_t */
static size_t cache_cap = 0;       /* current allocated capacity */
static idx_t  cache_max = 0;       /* highest n already computed */

/* Helper to expand the cache to at least max_index+1 */
static void
ensure_cache(idx_t max_index)
{
    if (cache == NULL) {
        /* initial allocation */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return;      /* allocation failure, caller will handle */
    }
    if ((size_t)(max_index + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)(max_index + 1) > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return;        /* realloc failure, keep old cache */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Computes a(n) for n >= 1.  Returns 0 for n < 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Expand cache if necessary */
    if (n > cache_max) {
        ensure_cache(n);
        if (cache == NULL) return 0;   /* allocation failed */

        /* Initialise or compute new terms up to n */
        static const seq_t init_vals[8] = {
            0,   /* unused, 0-th index */
            10,  /* a(1) */
            68,  /* a(2) */
            136, /* a(3) */
            232, /* a(4) */
            360, /* a(5) */
            548, /* a(6) */
            834  /* a(7) */
        };

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 7) {
                cache[i] = init_vals[i];
            } else {
                /* Compute via reordered expression to avoid unsigned underflow:
                   cache[i] = 4*a(i-1) + 4*a(i-4) - (4*a(i-2) + 2*a(i-3) + a(i-6))
                */
                seq_t pos = cache[i-1] * (seq_t)4 + cache[i-4] * (seq_t)4;
                seq_t sub = cache[i-2] * (seq_t)4 + cache[i-3] * (seq_t)2
                            + cache[i-6];
                cache[i] = pos - sub;
            }
        }
        cache_max = n;
    }

    return cache[n];
}
