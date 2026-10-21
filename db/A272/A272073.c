/*
 * A272073  Exponents of x in the numerator of cluster variables of a rank 2
 *          cluster algebra.
 * https://oeis.org/A272073
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - a(n-4), with a(2)=0, a(3)=3, a(4)=6, a(5)=15
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 68, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For indices below the offset the value is irrelevant */
    if (n < 2)
        return 0;

    /* Static memoisation cache */
    static seq_t *cache      = NULL;
    static idx_t capacity    = 0;
    static idx_t max_cached  = 5;   /* highest index with a known value */

    /* Ensure the cache can accommodate index n */
    if (cache == NULL) {
        /* First allocation: at least up to index 5 */
        capacity = (n > 5) ? n + 1 : 6;
        cache    = (seq_t *)realloc(NULL, capacity * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failure – improbable in contest settings */

        /* Initialise the known base values */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 3;
        cache[4] = 6;
        cache[5] = 15;
    } else if (n + 1 > capacity) {
        /* Grow the cache if needed */
        idx_t new_capacity = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure */
        cache      = tmp;
        capacity   = new_capacity;
    }

    /* Compute all missing terms up to n */
    if (n > max_cached) {
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            /* i >= 6 here, so i-2 and i-4 are already defined */
            cache[i] = 4 * cache[i - 2] - cache[i - 4];
        }
        max_cached = n;
    }

    return cache[n];
}
