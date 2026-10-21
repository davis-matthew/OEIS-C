/*
 * A251374  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to 5 6 or 7.
 * https://oeis.org/A251374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 3*a(n-2) - 66*a(n-3) + 24*a(n-4) a(1)=124,
 *            a(2)=1048, a(3)=9124, a(4)=80296 (OEIS A251374)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;   /* pointer to cached values (0‑based: a(n+1)==cache[n]) */
static size_t cache_len = 0;  /* number of values currently stored */
static size_t cache_cap = 0;  /* allocated capacity of cache */

/* compute a(n), 1‑based index */
seq_t a(idx_t n)
{
    /* n must be >=1; behaviour for n==0 is undefined for this sequence */
    if (n == 0) return (seq_t)0;

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 4;                        /* at least four terms needed for the recurrence */
        cache     = seq_alloc(cache_cap);     /* allocate array of seq_t */
        cache[0] = (seq_t)124;
        cache[1] = (seq_t)1048;
        cache[2] = (seq_t)9124;
        cache[3] = (seq_t)80296;
        cache_len = 4;
    }

    /* grow the cache if necessary */
    if ((size_t)n > cache_cap) {
        cache = seq_realloc(cache, cache_cap, (size_t)n);
        cache_cap = (size_t)n;
    }

    /* compute missing terms up to index n */
    if ((size_t)n > cache_len) {
        for (idx_t idx = cache_len; idx < n; ++idx) { /* idx is 0‑based for a(idx+1) */
            /* idx >= 4 because the first 4 terms are already stored */
            seq_t val =
                ((seq_t)10 * cache[idx-1]) -
                ((seq_t)3  * cache[idx-2]) -
                ((seq_t)66 * cache[idx-3]) +
                ((seq_t)24 * cache[idx-4]);
            cache[idx] = val;
        }
        cache_len = (size_t)n;
    }

    return cache[n-1];
}
