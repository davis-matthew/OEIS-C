/*
 * A310999  Coordination sequence Gal.4.7.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)-2*a(n-2)+2*a(n-3)-2*a(n-4)+2*a(n-5)-2*a(n-6)
 *            +2*a(n-7)-2*a(n-8)+2*a(n-9)-a(n-10) (n > 13) Initial values
 *            (offset 0): a(0)=1, a(1)=4, a(2)=8, a(3)=10, a(4)=15, a(5)=20,
 *            a(6)=23, a(7)=30, a(8)=32, a(9)=34, a(10)=40, a(11)=44,
 *            a(12)=50, a(13)=52
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* Static vector to hold the sequence.  It is automatically
       value‑initialised for the arbitrary‑precision type.          */
    static std::vector<seq_t> cache = {1,4,8,10,15,20,23,30,32,34,40,44,50,52};
    /* Expand the vector if the requested index is beyond the
       current size.                                              */
    if (cache.size() < (size_t)(n+1)) cache.resize(n+1);

    /* Compute missing values up to n.  The recurrence is valid
       for n >= 14.  All indices are valid because the vector
       already contains the first 14 terms.                     */
    for (idx_t i = 14; i <= n; ++i) {
        seq_t pos = cache[i-1] + cache[i-3] + cache[i-5] +
                    cache[i-7] + cache[i-9];
        pos = pos + pos;                                 /* * 2 */
        seq_t neg = cache[i-2] + cache[i-4] + cache[i-6] +
                    cache[i-8];
        neg = neg + neg;                                 /* * 2 */
        neg = neg + cache[i-10];
        cache[i] = pos - neg;                            /* recurrence */
    }
    return cache[(size_t)n];
#else
    /* C implementation: use a dynamic array allocated with realloc.
       The array is stored as a static pointer. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static const seq_t init_vals[14] = {1,4,8,10,15,20,23,30,32,34,40,44,50,52};
    /* Initialise the array on the first call.  Allocate a small
       initial capacity and then populate the known values.   */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        for (size_t i = 0; i < 14; ++i) cache[i] = init_vals[i];
    }
    /* Ensure enough space for n+1 elements. */
    if (cache_cap < (size_t)(n+1)) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n+1)) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    /* Compute any missing terms. */
    for (idx_t i = 14; i <= n; ++i) {
        seq_t pos = cache[i-1] + cache[i-3] + cache[i-5] +
                    cache[i-7] + cache[i-9];
        pos = pos + pos;                                 /* * 2 */
        seq_t neg = cache[i-2] + cache[i-4] + cache[i-6] +
                    cache[i-8];
        neg = neg + neg;                                 /* * 2 */
        neg = neg + cache[i-10];
        cache[i] = pos - neg;                            /* recurrence */
    }
    return cache[(size_t)n];
#endif
}
