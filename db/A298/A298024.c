/*
 * A298024  Expansion of (x^4+3*x^3+6*x^2+3*x+1)/((1-x)*(1-x^3)).
 * https://oeis.org/A298024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=10, a(3)=14, a(4)=18, a(n) = a(n-1) +
 *            a(n-3) - a(n-4) for n > 4
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamically allocated cache */
static idx_t cache_size = 0;     /* number of entries already cached */

/* Compute a(n) using the recurrence and memoisation */
seq_t a(idx_t n) {
    /* Grow cache if needed */
    if (n >= cache_size) {
        idx_t new_size = cache_size > 0 ? cache_size * 2 : 8;
        if (new_size <= n) new_size = n + 1;

        seq_t *new_cache = NULL;
        if (cache == NULL)
            new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();  /* allocation failed */

        cache = new_cache;

        /* Initialise missing base values if they are in the new range */
        idx_t i;
        for (i = cache_size; i < new_size && i < 5; ++i) {
            if (i == 0) cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)4;
            else if (i == 2) cache[i] = (seq_t)10;
            else if (i == 3) cache[i] = (seq_t)14;
            else if (i == 4) cache[i] = (seq_t)18;
        }

        /* Compute new entries using the recurrence */
        for (i = cache_size; i < new_size; ++i) {
            if (i <= 4) continue;  /* base values already set */
            cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
        }

        cache_size = new_size;
    }

    return cache[n];
}
