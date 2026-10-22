/*
 * A366277  Fixed points of map n -> A366275(n).
 * https://oeis.org/A366277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the merged sorted list of 3·2^k and 55·2^k (k≥0), i.e.
 *            the smallest positive integer whose odd part is 3 or 55. The
 *            sequence is 3, 6, 12, 24, 48, 55, 96, 110, …
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h" /* seq_t *alloc_seq_array(size_t)   */
#include "../utils/free_seq_array.h"  /* void free_seq_array(seq_t *)    */

/* Cached sequence values */
static seq_t *cache     = NULL;   /* array of terms          */
static size_t cache_cap = 0;      /* allocated capacity      */
static idx_t  cache_len = 0;      /* number of terms stored  */

/* Current values in the two geometric sequences */
static seq_t cur1 = (seq_t)3;    /* 3·2^k   */
static seq_t cur2 = (seq_t)55;   /* 55·2^k  */

/* Ensure that the cache has space for at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;

    /* double the capacity (start at 16) */
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;

    /* allocate a new array */
    seq_t *new_cache = alloc_seq_array(new_cap);
    if (!new_cache) abort();   /* allocation failure */

    /* copy existing elements */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* free the old array */
    free_seq_array(cache);

    /* update pointers */
    cache     = new_cache;
    cache_cap = new_cap;
}

/* Return n-th term (n >= 1) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1; avoid UB */

    /* generate terms until we have at least n terms */
    while ((idx_t)cache_len < n)
    {
        /* insert the smaller of cur1 and cur2 */
        if (cur1 <= cur2)
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = cur1;
            cur1 = cur1 * (seq_t)2;    /* advance 3·2^k   */
        }
        else
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = cur2;
            cur2 = cur2 * (seq_t)2;    /* advance 55·2^k  */
        }
    }

    return cache[n - 1];
}
