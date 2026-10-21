/*
 * A250876  Number of (n+1) X (7+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 26, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4);
   Initial values: a(1)=5518, a(2)=27686, a(3)=131982, a(4)=600070 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    /* Static cache of computed terms: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* current number of entries stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure cache starts with the first four terms */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failed – unlikely in tests */
        cache[0] = (seq_t)5518;    /* a(1) */
        cache[1] = (seq_t)27686;   /* a(2) */
        cache[2] = (seq_t)131982;  /* a(3) */
        cache[3] = (seq_t)600070;  /* a(4) */
        cache_len = 4;
    }

    /* Extend cache up to needed index n */
    size_t target = (size_t)(n - 1);          /* zero‑based index for a(n) */
    while (cache_len <= target) {
        /* Compute next term using the recurrence */
        seq_t val;
        if (cache_len == 1)      /* a(2) already stored */
            val = cache[1];
        else if (cache_len == 2) /* a(3) already stored */
            val = cache[2];
        else if (cache_len == 3) /* a(4) already stored */
            val = cache[3];
        else {
            /* a(k+1) = 10*a(k) - 35*a(k-1) + 50*a(k-2) - 24*a(k-3) */
            val = (seq_t)10 * cache[cache_len - 1] -
                  (seq_t)35 * cache[cache_len - 2] +
                  (seq_t)50 * cache[cache_len - 3] -
                  (seq_t)24 * cache[cache_len - 4];
        }

        /* Resize cache if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) break;   /* allocation failure – return last known value */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = val;
    }

    return cache[target];
}
