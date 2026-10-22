/*
 * A345520  Numbers that are the sum of seven cubes in two or more ways.
 * https://oeis.org/A345520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: builds the list of numbers that are the sum of seven
   integer cubes in two or more distinct unordered ways.
   For each required n the list is extended by a dynamic programming
   routine that records, for every sum up to a bound, how many ways it
   can be expressed as a sum of exactly seven cubes.  Once the count is
   at least two the sum is stored.  The bound is doubled until the
   accumulated list contains at least n terms. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache of found terms */
static seq_t *cache          = NULL;   /* sorted list of sums with ≥2 representations */
static size_t cache_len      = 0;      /* number of terms stored in cache */
static idx_t cache_limit     = 0;      /* largest sum that has been processed */

/* Build or extend the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_len >= n) return;      /* already enough terms */

    /* Start with a modest bound and double it until we have enough terms. */
    idx_t limit = 100000;                   /* initial maximum sum to search */
    for (;;) {
        /* Allocate DP table: rows[used][sum] for used = 0..7. */
        size_t rowsz = (size_t)(limit + 1);
        unsigned char *dp = (unsigned char *)calloc(8 * rowsz, 1);
        if (!dp) return;                    /* allocation failure, give up */

        /* Pointers to each row. */
        unsigned char *rows[8];
        for (int r = 0; r < 8; ++r) rows[r] = dp + (size_t)r * rowsz;
        rows[0][0] = 1;                     /* one way to have zero cubes sum to 0 */

        /* Iterate over cube values (positive integers). */
        for (idx_t i = 1; ; ++i) {
            seq_t cube = (seq_t)i * (seq_t)i * (seq_t)i;
            if (cube > (seq_t)limit) break; /* beyond current bound */

            size_t cv = (size_t)cube;
            for (int used = 0; used <= 6; ++used) {
                unsigned char *src = rows[used];
                unsigned char *tgt = rows[used + 1];
                for (size_t s = cv; s <= (size_t)limit; ++s) {
                    unsigned char cnt = src[s - cv];
                    if (!cnt) continue;
                    unsigned char *p = &tgt[s];
                    if (*p < 2) {
                        *p += cnt;
                        if (*p > 2) *p = 2;   /* cap at 2 (we only need to know ≥2) */
                    }
                }
            }
        }

        /* Collect sums that have at least two representations. */
        size_t vec_cap = rowsz / 100 + 10;
        seq_t *vec = (seq_t *)malloc(vec_cap * sizeof(seq_t));
        if (!vec) { free(dp); return; }
        size_t vec_len = 0;

        unsigned char *row7 = rows[7];
        for (size_t s = 0; s <= (size_t)limit; ++s) {
            if (row7[s] >= 2) {
                if (vec_len == vec_cap) {
                    vec_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(vec, vec_cap * sizeof(seq_t));
                    if (!tmp) { free(vec); free(dp); return; }
                    vec = tmp;
                }
                vec[vec_len++] = (seq_t)s;
            }
        }

        free(dp);                    /* DP table no longer needed */

        /* If we have enough terms, store the list in the cache. */
        if (vec_len >= (size_t)n) {
            seq_t *new_cache = (seq_t *)malloc(vec_len * sizeof(seq_t));
            if (!new_cache) { free(vec); return; }

            for (size_t i = 0; i < vec_len; ++i) new_cache[i] = vec[i];
            free(vec);
            if (cache) free(cache);
            cache = new_cache;
            cache_len = vec_len;
            cache_limit = limit;
            return;
        }

        /* Not enough terms yet: discard vector, enlarge limit, and retry. */
        free(vec);
        limit <<= 1;               /* double current bound */
    }
}

/* The required function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour for n=0 is irrelevant */
    ensure_cache(n);                /* guarantee the cache holds at least n terms */
    return cache[n - 1];
}
