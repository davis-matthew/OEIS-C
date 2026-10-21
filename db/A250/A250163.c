/*
 * A250163  Number of length n+1 0..4 arrays with the sum of adjacent
 *          differences multiplied by some arrangement of +-1 equal to zero.
 * https://oeis.org/A250163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 16*a(n-1) -105*a(n-2) +372*a(n-3) -783*a(n-4) +1008*a(n-5) 
 *              -779*a(n-6) +332*a(n-7) -60*a(n-8)
 * Initial values: a(1)=5, a(2)=33, a(3)=211, a(4)=1269, a(5)=7109,
 *                 a(6)=37881, a(7)=195927, a(8)=996933
 * The sequence satisfies this recurrence for all n ≥ 9 (offset 1).
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;           /* number of terms cached (≥0) */

    /* initial terms (1-indexed) */
    static const seq_t init[8] = {
        (seq_t)5,
        (seq_t)33,
        (seq_t)211,
        (seq_t)1269,
        (seq_t)7109,
        (seq_t)37881,
        (seq_t)195927,
        (seq_t)996933
    };

    /* initialise cache on first call */
    if (cache_size == 0) {
        cache_size = 8;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory – caller must check */
        for (size_t i = 0; i < 8; ++i) cache[i] = init[i];
    }

    if (n <= (idx_t)cache_size) {
        return cache[n - 1];
    }

    /* extend cache to required size */
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;   /* out of memory */
    cache = new_cache;

    /* compute terms from cache_size+1 up to n using the recurrence */
    for (size_t k = cache_size + 1; k <= (size_t)n; ++k) {
        /* indices: a(k) uses terms a(k-1) .. a(k-8) */
        seq_t val = (seq_t)16 * cache[k - 2]
                  - (seq_t)105 * cache[k - 3]
                  + (seq_t)372 * cache[k - 4]
                  - (seq_t)783 * cache[k - 5]
                  + (seq_t)1008 * cache[k - 6]
                  - (seq_t)779 * cache[k - 7]
                  + (seq_t)332 * cache[k - 8]
                  - (seq_t)60 * cache[k - 9];
        cache[k - 1] = val;
    }

    cache_size = n;
    return cache[n - 1];
}
