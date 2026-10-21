/*
 * A250785  Number of (3+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250785: a(n) = 5*a(n‑1) – 8*a(n‑2) + 3*a(n‑3) + 3*a(n‑4) – 2*a(n‑5)
   (for n ≥ 6).  Initial values:
   a(1)=36, a(2)=96, a(3)=230, a(4)=534, a(5)=1194. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Basic sanity: the sequence is defined for n ≥ 1 only. */
    if (n == 0) return 0;

    /* Static cache for previously computed terms.
       cache[0] is unused to keep 1‑based indexing convenient. */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;   /* highest index computed so far */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_sz = 5;                                      // we know a(1)…a(5)
        cache = (seq_t *)malloc((cache_sz + 1) * sizeof(seq_t));
        if (!cache) return 0;   // allocation failure – should never happen
        /* Fill the known initial values. */
        cache[1] = 36;
        cache[2] = 96;
        cache[3] = 230;
        cache[4] = 534;
        cache[5] = 1194;
    }

    /* Ensure the cache is large enough. */
    size_t target = (size_t)n;
    if (target > cache_sz) {
        seq_t *new_cache = (seq_t *)realloc(cache, (target + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failure – return 0 as a safe fallback. */
            return 0;
        }
        cache = new_cache;

        /* Compute missing terms up to target using the recurrence. */
        for (size_t i = cache_sz + 1; i <= target; ++i) {
            seq_t v = 5 * cache[i - 1];
            v -= 8 * cache[i - 2];
            v += 3 * cache[i - 3];
            v += 3 * cache[i - 4];
            v -= 2 * cache[i - 5];
            cache[i] = v;
        }
        cache_sz = target;
    }

    return cache[n];
}
