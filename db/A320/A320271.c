/*
 * A320271  Number of unlabeled semi-binary rooted trees with n nodes in which
 *          the non-leaf branches directly under any given node are all equal.
 * https://oeis.org/A320271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(3)=2; For n≥4: if n is even:
 *            a(n)=a(n-1)+a(n-2); if n is odd: a(n)=a(n-1)+a(n-2)+a((n-1)/2)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based cache: cache[0] is unused */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* maximum index currently cached */

    /* Base case handling: if n<1 we return 0, although the sequence is
       defined only for n≥1. */
    if (n == 0)
        return (seq_t)0;

    /* Ensure at least 3 entries in the cache (our initial conditions). */
    if (cache_size < 3) {
        idx_t new_size = 3;
        seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* Allocation failed */
        cache = tmp;
        cache_size = new_size;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
    }

    /* Expand cache if needed. */
    if (n > cache_size) {
        idx_t old_size = cache_size;
        cache_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, (cache_size + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;

        /* Compute new terms strictly increasing. */
        for (idx_t k = old_size + 1; k <= n; ++k) {
            if (k <= 3) /* already set */ continue;
            if ((k & 1U) == 0) {               /* even */
                cache[k] = cache[k - 1] + cache[k - 2];
            } else {                            /* odd */
                idx_t m = (k - 1) >> 1;
                cache[k] = cache[k - 1] + cache[k - 2] + cache[m];
            }
        }
    }

    return cache[n];
}
