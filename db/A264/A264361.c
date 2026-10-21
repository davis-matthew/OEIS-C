/*
 * A264361  Number of (n+1) X (5+1) arrays of permutations of 0..n*6+5 with
 *          each element having directed index change 0,0 0,2 1,0 or -1,-2.
 * https://oeis.org/A264361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) + 22*a(n-2) - 8*a(n-3) for n >= 4, with initial
 *            values a(1)=36, a(2)=400, a(3)=5184
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes term a(n) of OEIS A264361.
   The function is memoized; calls may come in any order.
   seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* static cache: a[0] = a(1), a[1] = a(2), ... */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;   /* highest n for which cache[n-1] is valid */

    if (n < 1)
        return 0;          /* undefined for n=0, but return 0 as a safe default */

    /* First call: allocate space for at least 3 terms (the initial ones). */
    if (cache == NULL) {
        size_t cap = (size_t)((n > 3) ? n : 3);
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (cache == NULL)
            exit(1);   /* allocation failure */
        cache[0] = (seq_t)36;
        cache[1] = (seq_t)400;
        cache[2] = (seq_t)5184;
        max_cached = 3;
    }

    /* If we need terms beyond what we have, grow the cache and compute. */
    if (n > max_cached) {
        size_t new_cap = (size_t)n;            /* we only need up to n for now */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            exit(1);   /* allocation failure */
        cache = tmp;
        /* Compute missing terms using the recurrence. */
        for (idx_t k = max_cached + 1; k <= n; ++k) {
            cache[k - 1] = (seq_t)11 * cache[k - 2] +
                            (seq_t)22 * cache[k - 3] -
                            (seq_t)8  * cache[k - 4];
        }
        max_cached = n;
    }

    return cache[n - 1];
}
