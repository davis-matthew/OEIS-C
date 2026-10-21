/*
 * A291019  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - S - S^2 - S^3 + S^4.
 * https://oeis.org/A291019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=9, a(3)=25, a(4)=68,
 *            a(n)=5*a(n-1)-8*a(n-2)+6*a(n-3)-3*a(n-4) for n≥5
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t INIT_VALS[5] = { (seq_t)1, (seq_t)3, (seq_t)9, (seq_t)25, (seq_t)68 };

static seq_t *cache   = NULL;   /* memoized values */
static size_t cache_cap = 0;   /* allocated size */
static size_t cache_len = 0;   /* number of elements actually stored (≥5 after init) */
static int    initialized = 0;  /* flag whether INIT_VALS have been copied */

seq_t a(idx_t n)
{
    /* Return constant values directly for the first five indices. */
    if (n < 5)
        return INIT_VALS[(int)n];

    /* Allocate or grow cache to hold n+1 elements. */
    if (cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failure – give up */
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        } else {
            /* first allocation – zero-initialize */
            memset(new_cache, 0, new_cap * sizeof(seq_t));
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize the first five terms if not already done. */
    if (!initialized) {
        memcpy(cache, INIT_VALS, 5 * sizeof(seq_t));
        cache_len = 5;
        initialized = 1;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t j = (idx_t)cache_len; j <= n; ++j) {
        seq_t a1 = cache[j-1];
        seq_t a2 = cache[j-2];
        seq_t a3 = cache[j-3];
        seq_t a4 = cache[j-4];
        cache[j] = (seq_t)5 * a1 - (seq_t)8 * a2 + (seq_t)6 * a3 - (seq_t)3 * a4;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
