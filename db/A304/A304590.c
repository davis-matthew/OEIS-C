/*
 * A304590  a(1) = a(2) = a(3) = a(5) = 1, a(4) = 2, a(6) = 3; a(n) =
 *          a(n-a(n-3)) + a(n-a(n-6)) for n > 6.
 * https://oeis.org/A304590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-a(n-3)) + a(n-a(n-6)), a(1)=a(2)=a(3)=a(5)=1, a(4)=2,
 *            a(6)=3
 * Range:     u8 n <= 478, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Static memoization tables */
    static seq_t *cache = NULL;   /* holds a(1)..a(computed) */
    static idx_t cache_size = 0;  /* allocated size of cache */
    static idx_t computed = 0;    /* highest index computed so far */

    /* Ensure capacity for at least n elements */
    if (n > cache_size) {
        idx_t new_size = n * 2;  /* Grow roughly twice the size */
        if (new_size < 16) new_size = 16;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) { /* allocation failed */
            /* In the unlikely event of failure, reset cache to zero */
            free(cache);
            cache = NULL;
            cache_size = 0;
            computed = 0;
            /* Returning 0 as fallback; sequence undefined on OOM */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1 || i == 2 || i == 3 || i == 5) {
            cache[i - 1] = (seq_t)1;
        } else if (i == 4) {
            cache[i - 1] = (seq_t)2;
        } else if (i == 6) {
            cache[i - 1] = (seq_t)3;
        } else {
            /* Recurrence computation.
               All indices involved are < i, so the needed values are already cached. */
            idx_t off1   = (idx_t)cache[(i - 3) - 1]; /* a(i-3) */
            idx_t off2   = (idx_t)cache[(i - 6) - 1]; /* a(i-6) */
            idx_t idx1   = i - off1;                 /* n - a(n-3) */
            idx_t idx2   = i - off2;                 /* n - a(n-6) */
            /* Safety check is omitted: the recurrence guarantees idx1, idx2 >= 1. */
            cache[i - 1] = cache[idx1 - 1] + cache[idx2 - 1];
        }
    }
    computed = n;
    return cache[n - 1];
}
