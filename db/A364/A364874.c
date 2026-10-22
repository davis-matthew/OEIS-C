/*
 * A364874  A packing coloring for the one-way infinite path using only the
 *          first 8 prime numbers.
 * https://oeis.org/A364874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by iteratively selecting the smallest prime in
 * S={2,3,5,7,11,13,17,19} such that its previous occurrence is more
 * than that prime steps earlier.  The sequence is cached to allow
 * arbitrary order of calls. */
#include <stdlib.h>
#include <stddef.h>

/* Prime colors used by the packing coloring. */
static const idx_t prime_colors[8] = {2, 3, 5, 7, 11, 13, 17, 19};

/* Cache of computed terms.  cache[0] is unused; valid indices start at 1. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated capacity (elements) */
static size_t cache_len  = 0;   /* highest index that has been stored */

/* For each color, the last position where it was used.  0 means unused. */
static idx_t last_pos[8] = {0};

seq_t a(idx_t n)
{
    /* Handle the (unlikely) request for n == 0. */
    if (n == 0)
        return (seq_t)0;

    /* If value already cached, return it. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n];

    /* Ensure cache can hold the new element. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute all missing terms up to n inclusively. */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        for (int k = 0; k < 8; ++k) {
            idx_t s = prime_colors[k];
            /* Check if this prime can be used at position i. */
            if (last_pos[k] == 0 || (i - last_pos[k] > s)) {
                last_pos[k] = i;                       /* record use */
                cache[(size_t)i] = (seq_t)s;           /* store result */
                break;                                 /* move to next i */
            }
        }
    }

    cache_len = (size_t)n;
    return cache[(size_t)n];
}
