/*
 * A357484  Number of linearity regions of a max-pooling function with a 3 by
 *          n input and 2 by 2 pooling windows.
 * https://oeis.org/A357484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 31*a(n-2) + 20*a(n-3) - 4*a(n-4) for n >= 5
 *            Initial values: a(1) = 1 a(2) = 14 a(3) = 150 a(4) = 1536
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] unused */
static idx_t cache_size = 0;     /* largest index computed */

/* Compute a(n) using memoised recurrence */
seq_t a(idx_t n)
{
    /* Defensive: ignore n < 1 (offset starts at 1) */
    if (n < 1) return 0;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_size = 4;
        /* allocate space for indices 0..4 (5 elements) */
        cache = (seq_t*)malloc((cache_size + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;     /* allocation failure */

        /* store known initial values */
        cache[1] = 1;
        cache[2] = 14;
        cache[3] = 150;
        cache[4] = 1536;
    }

    /* If the requested index is already cached, return it */
    if (n <= cache_size) return cache[n];

    /* Grow the cache to accommodate up to index n */
    idx_t new_size = n;
    seq_t *tmp = (seq_t*)realloc(cache, (new_size + 1) * sizeof(seq_t));
    if (tmp == NULL) return 0;        /* allocation failure */
    cache = tmp;

    /* Compute new terms iteratively, using only previously cached values */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t t1 = cache[i-1] * (seq_t)13 + cache[i-3] * (seq_t)20;
        seq_t t2 = cache[i-2] * (seq_t)31 + cache[i-4] * (seq_t)4;
        cache[i] = t1 - t2;   /* t1 >= t2 for all valid n */
    }

    cache_size = n;
    return cache[n];
}
