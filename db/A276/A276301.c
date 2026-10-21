/*
 * A276301  Number of 4 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-1,1) or (0,-1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A276301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=14, a(2)=31, a(3)=189, a(4)=1017, a(5)=5909,
   a(6)=33461, a(7)=191289, a(8)=1089941, for n>8
   a(n)=4*a(n-1)+10*a(n-2)-6*a(n-4)-22*a(n-5)+15*a(n-6) */
#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#  include <new>
#endif

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;     /* allocated space indexed by 1.. */
    static idx_t cache_cap    = 0;  /* number of allocated entries (index 0 unused) */
    static idx_t last_computed = 0; /* highest n that has been computed */

    if (n < 1) {
        /* Offset: a(1) is the first term; for safety return zero */
        return (seq_t)0;
    }

    /* Make sure we have room for index n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – return zero (depends on seq_t supporting cast) */
            return (seq_t)0;
        }
        /* Construct any new elements if seq_t is non‑POD */
        #ifdef __cplusplus
        for (idx_t i = cache_cap; i < new_cap; ++i) {
            new(&new_cache[i]) seq_t();
        }
        #endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        switch (i) {
            case 1: cache[1] = (seq_t)14; break;
            case 2: cache[2] = (seq_t)31; break;
            case 3: cache[3] = (seq_t)189; break;
            case 4: cache[4] = (seq_t)1017; break;
            case 5: cache[5] = (seq_t)5909; break;
            case 6: cache[6] = (seq_t)33461; break;
            case 7: cache[7] = (seq_t)191289; break;
            case 8: cache[8] = (seq_t)1089941; break;
            default:
                /* Recurrence for i > 8 */
                cache[i] = (seq_t)4 * cache[i - 1]
                        + (seq_t)10 * cache[i - 2]
                        - (seq_t)6 * cache[i - 4]
                        - (seq_t)22 * cache[i - 5]
                        + (seq_t)15 * cache[i - 6];
                break;
        }
    }
    last_computed = (last_computed < n) ? n : last_computed;

    return cache[n];
}
