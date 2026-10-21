/*
 * A298683  Start with the square tile of the Shield tiling and recursively
 *          apply the substitution rule. a(n) is the number of squares after n
 *          iterations.
 * https://oeis.org/A298683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2) - 9*a(n-3) + 2*a(n-4) for n>3 Initial
 *            values: a(0)=a(1)=a(2)=1, a(3)=13
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache   = NULL;   /* array of a(0) … a(max_computed)   */
    static size_t cap     = 0;      /* allocated capacity of cache        */
    static idx_t  last_n  = (idx_t)(-1); /* last index that has been computed */

    /* --- initialise cache on first call --------------------------------- */
    if (cache == NULL) {
        /* reserve space for the first four terms (indices 0..3) */
        cap = 4;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* graceful failure */

        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)1;   /* a(2) */
        cache[3] = (seq_t)13;  /* a(3) */
        last_n = 3;
    }

    /* --- expand cache if the requested index exceeds current size ----- */
    if (n > last_n) {
        /* ensure capacity up to n+1 */
        size_t target = (size_t)n + 1;
        if (target > cap) {
            size_t new_cap = cap * 2;
            if (new_cap < target) new_cap = target;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* graceful failure */
            cache = tmp;
            cap = new_cap;
        }

        /* compute new terms using the recurrence */
        for (size_t i = (size_t)(last_n + 1); i <= (size_t)n; ++i) {
            seq_t v1 = cache[i - 1];
            seq_t v2 = cache[i - 2];
            seq_t v3 = cache[i - 3];
            seq_t v4 = cache[i - 4];
            seq_t value = (seq_t)3 * v1 + (seq_t)5 * v2
                        - (seq_t)9 * v3 + (seq_t)2 * v4;
            cache[i] = value;
        }
        last_n = n;
    }

    return cache[n];
}
