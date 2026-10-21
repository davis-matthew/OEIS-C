/*
 * A297870  Number of nX2 0..1 arrays with every element equal to 2, 3 or 5
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 4*a(n-4) with initial values a(1)=0,
 *            a(2)=1, a(3)=3, a(4)=2 (recurrence holds for n >= 5)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage required */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;          /* array of computed values */
    static idx_t cache_n = 0;            /* highest index computed */
    static idx_t cache_cap = 0;          /* allocated capacity (index max) */

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        cache_cap = 4;                   /* we need indices 1..4 at least */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) return 0;           /* fallback on allocation failure */
        cache[0] = 0;                   /* unused, keep zero */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)2;
        cache_n   = 4;
    }

    /* Grow cache if n exceeds current capacity */
    if (n > cache_cap) {
        /* Allocate at least n+1 entries */
        idx_t new_cap = n;
        if (new_cap < 4) new_cap = 4;
        seq_t *new_cache = (seq_t *)realloc(cache,
                                             (new_cap + 1) * sizeof(seq_t));
        if (!new_cache) return 0;       /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    if (n > cache_n) {
        idx_t i;
        for (i = cache_n + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) val = (seq_t)0;
            else if (i == 2) val = (seq_t)1;
            else if (i == 3) val = (seq_t)3;
            else if (i == 4) val = (seq_t)2;
            else               /* recurrence for i >= 5 */
                val = cache[i - 1] + (seq_t)3 * cache[i - 2]
                      - (seq_t)4 * cache[i - 4];
            cache[i] = val;
        }
        cache_n = n;
    }

    return cache[n];
}
