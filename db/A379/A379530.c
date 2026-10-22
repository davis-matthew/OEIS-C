/*
 * A379530  a(n) = (A135318(3*n) + A135318(3*n+1) + A135318(3*n+2))/3.
 * https://oeis.org/A379530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=8, a(3)=23, a(n)=7*a(n-2)+8*a(n-4) for n>=4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


static seq_t *cache = NULL;          /* dynamically grown cache of values */
static idx_t  cache_size = 0;        /* number of entries available (>=0) */

/* Grow the cache so it can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)          /* already large enough */
        return;

    /* Size needed: at least n+1 entries */
    idx_t new_size = n + 1;
    if (new_size < 4)            /* base case ensures at least 4 entries */
        new_size = 4;

    /* Allocate a new buffer */
    seq_t *new_cache = (seq_t*)calloc((size_t)new_size, sizeof(seq_t));
    if (!new_cache)
        return;  /* allocation failure – caller must handle */

    /* Copy existing values if any */
    idx_t old_size = cache_size;
    for (idx_t i = 0; i < old_size; ++i)
        new_cache[i] = cache[i];

    /* Free old cache and install the new one */
    free(cache);
    cache = new_cache;
    cache_size = new_size;

    /* --- fill base cases if they were not already present --- */
    if (old_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)23;
        old_size = 4;            /* indices 0..3 are now defined */
    }

    /* --- compute remaining terms up to n --- */
    for (idx_t i = old_size; i <= n; ++i) {
        /* recurrence valid for i >= 4 */
        cache[i] = (seq_t)7 * cache[i - 2] + (seq_t)8 * cache[i - 4];
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
