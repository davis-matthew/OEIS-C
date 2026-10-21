/*
 * A296719  Number of n X 2 0..1 arrays with each 1 adjacent to 0, 2 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 2*a(n-2) + 6*a(n-3),
   with initial values a(1)=3, a(2)=9, a(3)=21. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* cached values, index 0 unused */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    if (n < 1) return 0; /* undefined for n=0, return 0 */

    /* lazily allocate and seed the cache */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t*)calloc((size_t)(cache_size + 1), sizeof(seq_t));
        if (cache == NULL) abort(); /* out of memory */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)21;
    }

    /* extend cache if necessary */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t*)realloc(cache,
                                            (size_t)(new_size + 1) * sizeof(seq_t));
        if (new_cache == NULL) abort(); /* out of memory */
        cache = new_cache;
        for (idx_t i = cache_size + 1; i <= new_size; ++i) {
            cache[i] = cache[i - 1]
                     + ((seq_t)2 * cache[i - 2])
                     + ((seq_t)6 * cache[i - 3]);
        }
        cache_size = new_size;
    }

    return cache[n];
}
