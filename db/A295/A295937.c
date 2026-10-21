/*
 * A295937  Number of n X 2 0..1 arrays with each 1 adjacent to 1 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A295937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 9*a(n-3) - 4*a(n-4) - 4*a(n-5) -
 *            4*a(n-6) with a(1)=2, a(2)=8, a(3)=15, a(4)=33, a(5)=104,
 *            a(6)=228
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
    /* For C++: allow use of 'new' and 'delete' to manage the cache. */
    #include <new>
#endif

/* Helpers for allocating and freeing dynamic arrays of type seq_t.
   They work in both C and C++. */
static inline seq_t *alloc_seq_array(idx_t size)
{
#ifdef __cplusplus
    return new seq_t[size];
#else
    return (seq_t*)malloc((size_t)size * sizeof(seq_t));
#endif
}

static inline void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Sequence of initial values (1‑based indexing) */
    static const seq_t init6[6] = 
        { (seq_t)2, (seq_t)8, (seq_t)15,
          (seq_t)33, (seq_t)104, (seq_t)228 };

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* number of allocated slots (index 0 unused) */
    static idx_t max_cached = 0; /* largest index that has been computed */

    /* Expand cache if necessary */
    if (n + 1 > capacity) { /* need array size at least n+1 (ignore index 0) */
        idx_t new_cap = n + 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache) abort();          /* allocation failed */

        /* copy existing values to new cache */
        for (idx_t i = 1; i <= max_cached; ++i)
            new_cache[i] = cache[i];

        /* initialise the rest of the new cache to 0 */
        for (idx_t i = max_cached + 1; i < new_cap; ++i)
            new_cache[i] = (seq_t)0;

        /* free old cache */
        if (cache)
            free_seq_array(cache);

        cache = new_cache;
        capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    if (n > max_cached) {
        /* Fill initial values that have not yet been set */
        for (idx_t i = max_cached + 1; i <= 6 && i <= n; ++i)
            cache[i] = init6[i - 1];

        /* Recurrence for i > 6 */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i <= 6) {
                /* already handled */
                continue;
            }
            seq_t val = cache[i - 1] + cache[i - 2]
                      + (seq_t)9 * cache[i - 3];
            val -= (seq_t)4 * cache[i - 4];
            val -= (seq_t)4 * cache[i - 5];
            val -= (seq_t)4 * cache[i - 6];
            cache[i] = val;
        }
        max_cached = n;
    }

    return cache[n];
}
