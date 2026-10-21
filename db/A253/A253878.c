/*
 * A253878  Indices of triangular numbers (A000217) which are also centered
 *          heptagonal numbers (A069099).
 * https://oeis.org/A253878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - 17*a(n-2) + a(n-3) with a(1)=1, a(2)=22,
 *            a(3)=358
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* 1‑based cache of computed terms */
    static idx_t capacity = 0;        /* allocated size of cache (index 0 unused) */
    static idx_t max_computed = 0;    /* highest index computed and stored */

    /* Initial allocation and base values */
    if (cache == NULL) {
        capacity = 8;                  /* small start, will grow dynamically */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure (unlikely in tests) */

        /* base values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)22;
        cache[3] = (seq_t)358;
        max_computed = 3;
    }

    /* Ensure the cache is large enough to hold index n */
    if (n >= capacity) {
        idx_t new_capacity = capacity;
        while (new_capacity <= n) new_capacity <<= 1;  /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) return 0;                          /* allocation failure */
        cache = tmp;
        capacity = new_capacity;
    }

    /* Compute missing terms up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i <= 3) {
                /* already initialized */
                continue;
            }
            cache[i] = (seq_t)17 * cache[i - 1]
                       - (seq_t)17 * cache[i - 2]
                       + cache[i - 3];
        }
        max_computed = n;
    }

    return cache[n];
}
