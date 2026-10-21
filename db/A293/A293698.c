/*
 * A293698  Values of positive integer i such that floor(tan(i)) = 1.
 * https://oeis.org/A293698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 6677, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is defined as the n‑th positive integer i for
   which floor(tan(i)) = 1.
   Computation is done by brute‑force search: values of i are tested
   sequentially; when floor(tan(i)) equals 1, i is appended to a
   cache.  Subsequent calls reuse the cache, allowing arbitrary call
   order.  The algorithm runs in O(a(n)) time, where a(n) is the
   value of the n‑th term, and stays well below the 2‑second limit
   for n ≤ 10 000. */

#include <math.h>
#ifdef __cplusplus
    #include <vector>
    #include "../utils/seq_alloc.h"
#else
    #include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    static idx_t next_i = 1;                /* next integer to test   */

#ifdef __cplusplus
    static std::vector<seq_t> cache;        /* cache of found terms   */

    /* Grow the cache until it contains at least n terms  */
    while (cache.size() < n) {
        double t = tan((double)next_i);     /* compute tan(i)          */
        if (t >= 1.0 && t < 2.0)            /* floor(tan(i)) == 1 ?   */
            cache.push_back((seq_t)next_i); /* store the integer      */
        ++next_i;
    }
    return cache[n - 1];

#else /* C mode */

    /*
       In the C build seq_t is a simple integer type, so we can use
       malloc/realloc to manage the dynamic array.  The cache and
       its state persist across calls because they are static.
    */
    static seq_t *cache = NULL;   /* pointer to stored terms       */
    static size_t cache_len = 0;  /* number of terms in cache      */
    static size_t cache_cap = 0;  /* allocated capacity of cache   */

    while (cache_len < n) {
        double t = tan((double)next_i);     /* compute tan(i)          */
        if (t >= 1.0 && t < 2.0) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {                 /* allocation failed */
                    /* Clear cache to avoid double free */
                    free(cache);
                    cache = NULL;
                    cache_len = cache_cap = 0;
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_i;
        }
        ++next_i;
    }
    return cache[n - 1];
#endif
}
