/*
 * A317760  Number of nX4 0..1 arrays with every element unequal to 0, 1 or 2
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A317760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=8, a(2)=20, a(3)=28, a(4)=43, a(5)=72, a(6)=127 a(n) =
 *            2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4) + a(n-5) for n>6
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 67, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;   /* largest n computed so far */
    static idx_t cap   = 0;   /* allocated size for cache (index 0..cap-1) */

    /* initialise base values once */
    if (max_n == 0) {
        /* allocate space for indices 0..6 (index 0 unused) */
        cap = 7;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure, return 0 as fallback */
        cache[0] = 0;            /* unused placeholder */
        cache[1] = 8;
        cache[2] = 20;
        cache[3] = 28;
        cache[4] = 43;
        cache[5] = 72;
        cache[6] = 127;
        max_n = 6;
    }

    if (n <= max_n)
        return cache[n];

    /* enlarge cache if necessary (at least up to n+1 entries) */
    if (cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        cache[i] =
            2 * cache[i-1] + cache[i-2]
            - 2 * cache[i-3] - cache[i-4] + cache[i-5];
    }
    max_n = n;
    return cache[n];
}
