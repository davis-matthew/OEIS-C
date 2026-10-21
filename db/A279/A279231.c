/*
 * A279231  Expansion of g.f. 1/((1 - x)^2*(1 - 3*x + 3*x^2)).
 * https://oeis.org/A279231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=15, a(3)=34
 *            a(n)=5*a(n-1)-10*a(n-2)+9*a(n-3)-3*a(n-4) for n>3 (OEIS A279231)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 37, i64 n <= 76, i128 n <= 158,
 *            big any n
 * Helpers:   malloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/malloc_seq.h"

static seq_t *cache = NULL;          /* dynamic cache of computed terms */
static idx_t  computed_upto = 0;     /* highest index already computed */
static size_t cache_capacity = 0;    /* allocated size of cache */

/* Grow the cache to at least new_cap entries, copying the existing
   computed values across. */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_capacity)
        return;

    seq_t *new_cache = malloc_seq(new_cap);
    if (cache != NULL) {
        /* copy previously computed terms */
        for (size_t i = 0; i <= (size_t)computed_upto; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise cache with the first four terms if it hasn't been
       initialised yet. */
    if (cache == NULL) {
        /* Allocate capacity for at least the first 4 terms. */
        cache_capacity = 4;
        cache = malloc_seq(cache_capacity);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)34;
        computed_upto = 3;
    }

    /* If the requested index is already computed, return it immediately. */
    if (n <= computed_upto)
        return cache[n];

    /* Grow cache if necessary to hold new terms up to n. */
    ensure_capacity((size_t)(n + 1));

    /* Compute new terms using the 4‑term linear recurrence. */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        cache[i] = 5 * cache[i - 1]               /* 5*a(n-1) */
                 - 10 * cache[i - 2]             /* -10*a(n-2) */
                 + 9  * cache[i - 3]             /* +9*a(n-3) */
                 - 3  * cache[i - 4];            /* -3*a(n-4) */
    }
    computed_upto = n;
    return cache[n];
}
