/*
 * A250577  Number of (n+1) X (2+1) 0..1 arrays with nondecreasing
 *          max(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 50, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 4*a(n-1) - 3*a(n-2) - 5*a(n-3) + 8*a(n-4) - 3*a(n-5) for n>=6
 * Initial values: a(1)=24, a(2)=60, a(3)=142, a(4)=329, a(5)=758
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t max_cached = 0;

    if (n > max_cached) {
        /* Ensure the cache can hold indices up to n (1‑based) */
        size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t new_cap = cap == 0 ? 8 : cap * 2;
            if (new_cap < need) new_cap = need;

            seq_t *tmp = NULL;
            if (cache == NULL) {
                tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
            } else {
                tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            }
            if (!tmp) return 0;            /* allocation failed – return 0 */
            cache = tmp;
            cap = new_cap;
            for (size_t i = max_cached + 1; i < cap; ++i) cache[i] = 0;
        }

        /* Compute missing terms up to n */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            switch (i) {
                case 1: cache[1] = (seq_t)24; break;
                case 2: cache[2] = (seq_t)60; break;
                case 3: cache[3] = (seq_t)142; break;
                case 4: cache[4] = (seq_t)329; break;
                case 5: cache[5] = (seq_t)758; break;
                default:
                    cache[i] = (seq_t)4 * cache[i-1]
                             - (seq_t)3 * cache[i-2]
                             - (seq_t)5 * cache[i-3]
                             + (seq_t)8 * cache[i-4]
                             - (seq_t)3 * cache[i-5];
                    break;
            }
        }
        max_cached = n;
    }

    return cache[n];
}
