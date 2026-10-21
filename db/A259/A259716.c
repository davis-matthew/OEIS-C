/*
 * A259716  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 or 00000011.
 * https://oeis.org/A259716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 33, u64 n <= 71, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-3) + 3*a(n-4) for n ≥ 5
   Initial values: a(1)=32, a(2)=54, a(3)=140, a(4)=242 */

#include <stdlib.h>  /* malloc, realloc, free, abort */

static seq_t *cache = NULL;   /* cached values; unused index 0 */
static idx_t cache_len = 0;   /* highest index that has been computed */

/* Ensure that cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    /* Grow cache allocation if needed. */
    size_t new_cap = (size_t)(n + 1);          /* need indices 0..n inclusive */
    if (new_cap > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();                           /* out of memory */
        cache = new_cache;
    }

    /* Compute new entries in ascending order. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)32;
        else if (i == 2)
            cache[i] = (seq_t)54;
        else if (i == 3)
            cache[i] = (seq_t)140;
        else if (i == 4)
            cache[i] = (seq_t)242;
        else
            cache[i] = (seq_t)4 * cache[i - 3] + (seq_t)3 * cache[i - 4];
    }

    cache_len = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;       /* outside the defined offset */
    ensure_cache(n);
    return cache[n];
}
