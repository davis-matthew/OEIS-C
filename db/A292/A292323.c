/*
 * A292323  p-INVERT of (1,0,0,1,0,0,1,0,0,...), where p(S) = (1 - S)(1 +
 *          S^2).
 * https://oeis.org/A292323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + 4*a(n-3) - 2*a(n-4) + a(n-5) - 3*a(n-6)
 *            * + a(n-7) + a(n-9) for n >= 10, * with initial values * a(0) =
 *            1, a(1) = 0, a(2) = 0, a(3) = 2, a(4) = 1, * a(5) = 0, a(6) = 5,
 *            a(7) = 6, a(8) = 1, a(9) = 11
 * Range:     u8 n <= 18, u16 n <= 32, u32 n <= 61, u64 n <= 119,
 *            u128 n <= 235, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed terms */
static idx_t  cache_size = 0;        /* number of terms stored in cache */

/* initial values for n = 0..9 */
static const seq_t init_vals[10] = {1,0,0,2,1,0,5,6,1,11};

seq_t a(idx_t n)
{
    /* return directly for the first 10 terms */
    if (n < 10)
        return init_vals[n];

    /* allocate and seed cache if it does not yet exist */
    if (cache == NULL) {
        cache_size = 10;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init_vals[i];
    }

    /* grow the cache and compute any missing terms up to n */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) {           /* allocation failed – abort */
            abort();
        }
        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = cache[i-1]                  /* a(n-1)   */
                       - cache[i-2]                   /* -a(n-2)  */
                       + (seq_t)4 * cache[i-3]        /* +4a(n-3) */
                       - (seq_t)2 * cache[i-4]        /* -2a(n-4) */
                       + cache[i-5]                   /* +a(n-5)  */
                       - (seq_t)3 * cache[i-6]        /* -3a(n-6) */
                       + cache[i-7]                   /* +a(n-7)  */
                       + cache[i-9];                  /* +a(n-9)  */
            cache[i] = val;
        }
        cache_size = new_size;
    }

    /* return the requested term */
    return cache[n];
}
