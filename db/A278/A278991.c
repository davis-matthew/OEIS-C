/*
 * A278991  a(n) is the number of simple linear diagrams with n+1 chords.
 * https://oeis.org/A278991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1)*a(n-1) + (4*n-3)*a(n-2) + (2*n-4)*a(n-3) Initial
 *            terms: a(0)=0, a(1)=1, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements stored */

    /* Ensure the cache is large enough to hold index n */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {   /* allocation failure – aborting is acceptable */
            /* In practice, we could handle this more gracefully */
            return 0;
        }
        cache = new_cache;
        /* Compute new entries */
        for (size_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 0;
            } else if (i == 1) {
                cache[1] = 1;
            } else if (i == 2) {
                cache[2] = 3;
            } else {
                seq_t t1 = ((seq_t)(2 * i - 1)) * cache[i - 1];
                seq_t t2 = ((seq_t)(4 * i - 3)) * cache[i - 2];
                seq_t t3 = ((seq_t)(2 * i - 4)) * cache[i - 3];
                cache[i] = t1 + t2 + t3;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
