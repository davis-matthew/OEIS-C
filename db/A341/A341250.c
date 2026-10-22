/*
 * A341250  a(n) = 5*a(n-1) - 4*a(n-3) for n >= 4, where a(1) = 1, a(2) = 3,
 *          a(3) = 13.
 * https://oeis.org/A341250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=3, a(3)=13; for n >= 4, a(n)=5*a(n-1)-4*a(n-3)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache of computed terms */
static idx_t cache_size = 0;  /* largest index stored */
static idx_t cache_cap  = 0;  /* maximum index the cache can hold */

/* Compute a(n) for n >= 1.  n==0 is outside the defined offset and returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Expand the cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = (cache_cap > 0) ? cache_cap : 4;
        while (new_cap < n)
            new_cap <<= 1;

        seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;  /* allocation failure */

        /* Copy existing values to the new storage */
        for (idx_t i = 1; i <= cache_size; ++i)
            new_cache[i] = cache[i];

        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Fill the cache up to the requested index */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1)
            cache[1] = (seq_t)1;
        else if (i == 2)
            cache[2] = (seq_t)3;
        else if (i == 3)
            cache[3] = (seq_t)13;
        else
            cache[i] = ((seq_t)5) * cache[i - 1] - ((seq_t)4) * cache[i - 3];
    }
    cache_size = n;

    return cache[n];
}
