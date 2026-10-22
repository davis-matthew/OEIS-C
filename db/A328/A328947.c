/*
 * A328947  Numbers formed from decimal digits 0 and/or 1 which are divisible
 *          by 7.
 * https://oeis.org/A328947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 146, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328947: numbers formed from decimal digits 0 or 1 that are
   divisible by 7.  The values are generated in increasing order by
   increasing decimal length.  The sequence is stored in a static cache
   so that calls with any order of indices are served.  This implementation
   examines all numbers of a given length before moving to the next
   length, guaranteeing that the cache is fully expanded for that
   length when a larger index is requested.  */
#include <stdbool.h>
#include <stdlib.h>

/* Grow the cache when necessary.  */
static void grow_cache(size_t *size, size_t *cap, seq_t **arr)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr   = tmp;
        *cap   = new_cap;
    }
}

/* Return the n‑th term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    /* Static state retained between calls. */
    static seq_t  *cache       = NULL;
    static size_t  cache_cap   = 0;
    static idx_t   cache_sz    = 0;
    static idx_t   max_len     = 1;   /* longest length processed fully */

    /* First call: initialise the cache with the first term a(1)=0. */
    if (cache_sz == 0) {
        grow_cache(&cache_sz, &cache_cap, &cache);
        cache[cache_sz++] = (seq_t)0;   /* a(1) = 0 */
    }

    /* Expand the cache until it contains at least n terms.  */
    while (cache_sz < n) {
        /* Process numbers of the next decimal length. */
        idx_t len = max_len + 1;
        idx_t start = ((idx_t)1 << (len - 1));
        idx_t end   = ((idx_t)1 << len);

        for (idx_t mask = start; mask < end; ++mask) {
            seq_t val = 0;
            for (idx_t i = len; i > 0; --i) {
                val = val * 10 + ((mask >> (i - 1)) & 1);
            }
            if ((val % (seq_t)7) == 0) {
                grow_cache(&cache_sz, &cache_cap, &cache);
                cache[cache_sz++] = val;
            }
        }

        max_len = len;   /* the whole block was processed */
    }

    return cache[n - 1];
}
