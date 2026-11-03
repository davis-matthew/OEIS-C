/*
 * A393464  Number of maximum independent vertex sets and minimum vertex
 *          covers in the n-necklace graph.
 * https://oeis.org/A393464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=4, a(2)=4, a(3)=24, a(4)=8, a(n)=4*a(n-2)-4*a(n-4) for n ≥
 *            5 (offset 1)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t* reallocate_array(seq_t* old, idx_t old_len, idx_t new_len)
{
#ifdef __cplusplus
    /* C++: use new[] which calls constructors for complex types */
    seq_t* new_arr = new seq_t[new_len];
#else
    /* C: use malloc with proper casting */
    seq_t* new_arr = (seq_t*)malloc((size_t)new_len * sizeof(seq_t));
#endif
    if (!new_arr) abort();

    /* copy any existing values */
    for (idx_t i = 0; i < old_len && i < new_len; ++i) {
        new_arr[i] = old[i];
    }

#ifdef __cplusplus
    if (old) delete[] old;
#else
    if (old) free(old);
#endif

    return new_arr;
}

seq_t a(idx_t n)
{
    /* external linkage; memoizes results in static array */
    static seq_t* cache = NULL;
    static idx_t cache_len = 0;          /* allocated size (index 0 unused) */

    if (n < 1) return 0;                 /* offset 1: a(1) is first term */

    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;            /* indices 0…n inclusive */
        seq_t* new_cache = reallocate_array(cache, cache_len, new_len);

        /* initialise newly allocated entries */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                new_cache[i] = 0;
            } else if (i == 1 || i == 2) {
                new_cache[i] = 4;
            } else if (i == 3) {
                new_cache[i] = 24;
            } else if (i == 4) {
                new_cache[i] = 8;
            } else {
                new_cache[i] = 4 * new_cache[i - 2] - 4 * new_cache[i - 4];
            }
        }

        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
