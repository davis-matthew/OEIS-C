/*
 * A314048  Coordination sequence Gal.6.618.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-10) for n > 10. Initial values: a(0)=1, a(1)=5, a(2)=11,
 *            a(3)=15, a(4)=21, a(5)=26, a(6)=31, a(7)=36, a(8)=41, a(9)=47,
 *            a(10)=51
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of allocated elements */

    /* Initial terms */
    static const seq_t init[11] = {
        1, 5, 11, 15, 21, 26, 31, 36, 41, 47, 51
    };

    /* Directly return if n is one of the initial values */
    if (n < 11)
        return init[(size_t)n];

    /* Need to compute up to n; grow cache if necessary */
    if (cache == NULL || n >= cache_size) {
        idx_t new_size = n + 1;          /* at least 0..n inclusive */
        seq_t *tmp;

        if (cache == NULL) {
            tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        if (!tmp)
            return 0;  /* allocation failure – unlikely under the constraints */

        /* If the allocation succeeded, remember old size */
        idx_t old_size = cache_size;
        cache = tmp;
        cache_size = new_size;

        /* Initialize (or re‑initialize) indices that just became available */
        idx_t i;
        for (i = old_size; i < new_size; ++i) {
            if (i < 11) {
                cache[i] = init[(size_t)i];
            } else {
                /* recurrence uses only earlier values (< i), which are now defined */
                cache[i] =
                    cache[i-1] + cache[i-3]
                    - cache[i-4] - cache[i-6]
                    + cache[i-7] + cache[i-9]
                    - cache[i-10];
            }
        }
    }

    /* All values up to n are now defined */
    return cache[(size_t)n];
}
