/*
 * A298344  a(n) = a(n-1) + a(n-2) + a([n/3]) + a([2n/3]), where a(0) = 1,
 *          a(1) = 1, a(2) = 1.
 * https://oeis.org/A298344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(floor(n/3)) + a(floor(2n/3)) where
 *            a(0) = a(1) = a(2) = 1. The function memoizes results so that
 *            any order of calls is handled
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* size_t */

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

static seq_t *cache = NULL;   /* memoized terms                         */
static size_t cap   = 0;      /* allocated capacity                    */
static size_t size  = 0;      /* number of terms currently stored      */

/* ------------------------------------------------------------------ */
/*  a – compute the n-th term of OEIS A298344                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* fast path for small indices (base cases) */
    if (n < 3)
        return (seq_t)1;

    /* initialization on first call */
    if (cache == NULL) {
        cap   = 3;
        cache = seq_alloc_new_array(cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        size   = 3;
    }

    /* if requested index is already stored, just return it */
    if ((size_t)n < size)
        return cache[n];

    /* grow the allocated array if needed   */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 8;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;          /* double until big enough */

        seq_t *new_cache = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < size; ++i)
            new_cache[i] = cache[i];
        seq_free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    for (size_t k = size; k <= (size_t)n; ++k) {
        seq_t t1 = cache[k - 1];              /* a(k-1)          */
        seq_t t2 = cache[k - 2];              /* a(k-2)          */
        seq_t t3 = cache[k / 3];              /* a(floor(k/3))   */
        seq_t t4 = cache[(k * 2) / 3];        /* a(floor(2k/3))  */
        cache[k] = t1 + t2 + t3 + t4;
    }
    size = (size_t)n + 1;
    return cache[n];
}
