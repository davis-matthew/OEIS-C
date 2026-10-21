/*
 * A247354  Number of paths from (0,1) to (n,0), with vertices (i,k)
 *          satisfying 0 <= k <= 3, consisting of segments given by the
 *          vectors (1,1), (1,2), (1,-1).
 * https://oeis.org/A247354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 69, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-2) + 2*a(n-3) - a(n-4);
   Initial values: a(0)=0, a(1)=1, a(2)=0, a(3)=2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the first four terms directly */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 0;
    if (n == 3) return 2;

    /* Dynamic cache for values ≥ 4 */
    static seq_t *cache = NULL;        /* array storing a(0), a(1), … */
    static size_t capacity = 0;        /* allocated size of cache       */
    static size_t computed = 0;        /* number of entries computed    */

    size_t idx = (size_t)n;            /* index into the cache          */

    /* Grow the cache if necessary */
    if (idx >= capacity) {
        size_t new_cap = idx + 5;      /* extra room to reduce reallocs */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* (void*) cast is allowed in C, needed in C++ */
        if (!tmp) abort();               /* out‑of‑memory failure          */
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise the first four known terms once */
    if (computed == 0) {
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 0;
        cache[3] = 2;
        computed = 4;                    /* a(0)…a(3) are ready           */
    }

    /* Compute missing terms up to idx inclusively */
    if (idx >= computed) {
        for (size_t i = computed; i <= idx; ++i) {
            cache[i] = 3 * cache[i-2] + 2 * cache[i-3] - cache[i-4];
        }
        computed = idx + 1;
    }

    return cache[idx];
}
