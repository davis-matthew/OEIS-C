/*
 * A275505  Number of 5 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,0) or (-1,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2) + a(n-3), a(1)=12, a(2)=56, a(3)=232
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS A275505.
 * The function is recursive linear and memoizes results so
 * that subsequent calls are O(1) for already computed indices. */
seq_t a(idx_t n)
{
    /* Handle the initial values explicitly. */
    if (n < 1) {
        return (seq_t)0;
    }
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)56;
    if (n == 3) return (seq_t)232;

    /* Static cache: 1‑indexed array of pre‑computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* maximum index stored in cache */

    /* Allocate cache if not yet done. */
    if (!cache) {
        cache = (seq_t *)calloc(4, sizeof(seq_t));   /* indices 0..3 */
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)56;
        cache[3] = (seq_t)232;
        cache_size = 3;
    }

    /* Grow the cache to hold at least up to index n. */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) { /* allocation failure – abort gracefully */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Compute new terms iteratively. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = (seq_t)3 * cache[i - 1]
                     + (seq_t)5 * cache[i - 2]
                     + cache[i - 3];
        }
        cache_size = n;
    }

    return cache[n];
}
