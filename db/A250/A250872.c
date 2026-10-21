/*
 * A250872  Number of (n+1) X (3+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4) a(1)=678,
 *            a(2)=3442, a(3)=16262, a(4)=73122. The recurrence is used for n
 *            ≥ 5. Results are cached to avoid recomputation for out‑of‑order
 *            calls
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 27, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values.  Index 0 is unused. */
    static seq_t *Cache = NULL;
    static size_t CacheSize = 0;   /* highest index stored in Cache */

    /* Initialise cache with the first four terms if it doesn't exist. */
    if (Cache == NULL) {
        CacheSize = 4;
        Cache = (seq_t *)malloc((CacheSize + 1) * sizeof(seq_t));
        if (Cache == NULL) return 0;     /* allocation failure */
        Cache[0] = 0;                    /* unused */
        Cache[1] = (seq_t)678;
        Cache[2] = (seq_t)3442;
        Cache[3] = (seq_t)16262;
        Cache[4] = (seq_t)73122;
    }

    /* Grow and fill the cache if a larger index is requested. */
    if ((size_t)n > CacheSize) {
        size_t newSize = (size_t)n;       /* highest needed index */
        seq_t *newCache = (seq_t *)malloc((newSize + 1) * sizeof(seq_t));
        if (newCache == NULL) return 0;   /* allocation failure */
        memcpy(newCache, Cache, (CacheSize + 1) * sizeof(seq_t));
        free(Cache);
        Cache = newCache;

        /* Compute missing terms using the recurrence. */
        for (size_t i = CacheSize + 1; i <= newSize; ++i) {
            seq_t val = (seq_t)(10) * Cache[i-1];
            val -= (seq_t)(35) * Cache[i-2];
            val += (seq_t)(50) * Cache[i-3];
            val -= (seq_t)(24) * Cache[i-4];
            Cache[i] = val;
        }
        CacheSize = newSize;
    }

    return Cache[n];
}
