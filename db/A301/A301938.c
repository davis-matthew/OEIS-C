/*
 * A301938  Numbers n with the property that n^2 contains a sequence of four
 *          or more consecutive 8's.
 * https://oeis.org/A301938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_run_of_8s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence defined by searching for n such that n² contains at least
   four consecutive 8 digits in base‑10.  Every call returns the n‑th
   element of the sequence, indexed from 1.  The function lazily
   builds a cache of found terms so that later queries are fast. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_run_of_8s.h"

/* static state: a simple dynamic array of seq_t values */
static seq_t *cache       = NULL;   /* cache of found terms          */
static size_t cache_size  = 0;       /* number of stored terms         */
static size_t cache_cap   = 0;       /* allocated capacity             */
static seq_t  last_checked = 0;      /* last n examined for the run    */

/* Grow the dynamic array to at least 'needed' elements.  The function
   doubles the current capacity (or starts at 8) until it satisfies the
   request, copies existing values, and frees the old buffer. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_buf) {
        /* Allocation failure – abort program as this is critical. */
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < cache_size; ++i)
        new_buf[i] = cache[i];

    free(cache);
    cache      = new_buf;
    cache_cap  = new_cap;
}

/* Return the n‑th term of A301938 (1‑based).  On first invocation the
   function populates the cache by scanning successive integers until
   the requested index is reached.  Subsequent calls reuse the cache. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1 – defensive return */

    /* Ensure the cache contains at least n terms. */
    while (cache_size < (size_t)n) {
        /* Scan the next integer for the required digit run. */
        ++last_checked;
        if (has_run_of_8s(last_checked)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = last_checked;
        }
    }
    return cache[(size_t)n - 1];
}
