/*
 * A299255  Coordination sequence for 3D uniform tiling formed by stacking
 *          parallel layers of the 3.3.4.3.4 2D tiling (cf. A219529).
 * https://oeis.org/A299255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 110, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 * a(0)=1, a(1)=7, a(2)=23, a(3)=50, a(4)=87, a(5)=135;
 * for n>5: a(n)=2*a(n-1)-a(n-2)+a(n-3)-2*a(n-4)+a(n-5)
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoization */
static seq_t *cache      = NULL;   /* array of computed terms */
static size_t cache_cap  = 0;      /* allocated size of cache array */
static size_t cache_len  = 0;      /* number of terms already stored */

/* Ensure the cache has capacity at least (n+1) */
static void
ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (need > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < need) new_cap *= 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_ptr == NULL) { /* allocation failure – terminate */
            exit(EXIT_FAILURE);
        }
        cache     = new_ptr;
        cache_cap = new_cap;
    }
}

/* Main entry point for the sequence */
seq_t
a(idx_t n)
{
    ensure_cache(n);

    /* Initialise initial terms on first use */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)23;
        cache[3] = (seq_t)50;
        cache[4] = (seq_t)87;
        cache[5] = (seq_t)135;
        cache_len = 6;
    }

    /* Return already computed value */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i <= 5) {
            /* initial values already set */
            continue;
        }
        seq_t term = cache[i - 1];           /* 2*a(i-1)  */
        term = term * 2;
        term = term - cache[i - 2];          /* -a(i-2)   */
        term = term + cache[i - 3];          /* +a(i-3)   */
        term = term - cache[i - 4] * 2;      /* -2*a(i-4) */
        term = term + cache[i - 5];          /* +a(i-5)   */
        cache[i] = term;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
