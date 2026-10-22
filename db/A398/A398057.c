/*
 * A398057  Upper (1,4/5) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7);
 *            (a(0),a(1),a(2),a(3),a(4),a(5),a(6)) = (0,2,7,14,23,35,50)
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values. */
    static seq_t *cache = NULL;      /* Pointer to cached sequence values   */
    static idx_t cache_sz = 0;       /* Number of valid elements in cache   */
    static idx_t cache_cap = 0;      /* Allocated capacity of cache         */

    /*--- Initialize cache with the first 7 values. -------------------*/
    if (cache_sz == 0) {
        /* Allocate initial capacity (at least 8 to be safe). */
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* in case of OOM, return 0 */

        /* Insert known initial values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)23;
        cache[5] = (seq_t)35;
        cache[6] = (seq_t)50;
        cache_sz = 7;  /* we have values up to index 6 */
    }

    /*--- Grow cache if necessary -------------------------------------*/
    if (n >= cache_cap) {
        /* Double capacity until we can hold n+1 elements. */
        idx_t new_cap = cache_cap;
        while (new_cap <= n) {
            new_cap <<= 1;                 /* multiply by 2 */
        }
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* OOM handling */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /*--- Compute values that are still missing -----------------------*/
    for (idx_t i = cache_sz; i <= n; ++i) {
        seq_t val;
        if (i <= 6) {
            /* Values 0..6 are already set in the initial block. */
            val = cache[i];
        } else {
            /* Apply the recurrence relation step by step to avoid
               overflow in intermediate computations. */
            val = cache[i-1];
            val = val + val;                /* 2*a(i-1)   */
            val = val - cache[i-2];         /* - a(i-2)   */
            val = val + cache[i-5];         /* + a(i-5)   */
            val = val - cache[i-6];         /* - a(i-6)   */
            val = val - cache[i-6];         /* - a(i-6)   */
            val = val + cache[i-7];         /* + a(i-7)   */
        }
        cache[i] = val;
    }
    cache_sz = n + 1;

    return cache[n];
}
