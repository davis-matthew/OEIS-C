/*
 * A282153  Expansion of x*(1 - 2*x + 3*x^2)/((1 - x)*(1 - 2*x)*(1 - x +
 *          x^2)).
 * https://oeis.org/A282153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 2*a(n-4) (n>3) a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=5
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      // Store computed terms
static size_t cache_size = 0;    // Size of allocated cache
static idx_t cache_max = (idx_t)-1; /* largest n cached; -1 means none yet */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;

    /* Initialise the cache if it does not exist yet */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache_max = 3;
    }

    /* Return if already cached */
    if (n <= cache_max)
        return cache[(size_t)n];

    /* Expand the cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)          /* allocation failed – return zero */
            return (seq_t)0;
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute terms beyond cache_max up to n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t val = 4 * cache[(size_t)(k-1)]
                  - 6 * cache[(size_t)(k-2)]
                  + 5 * cache[(size_t)(k-3)]
                  - 2 * cache[(size_t)(k-4)];
        cache[(size_t)k] = val;
    }
    cache_max = n;
    return cache[(size_t)n];
}
