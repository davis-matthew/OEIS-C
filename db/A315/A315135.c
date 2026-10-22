/*
 * A315135  Coordination sequence Gal.4.122.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=6, a(2)=10, a(3)=12, a(4)=20, a(5)=26, a(6)=32, a(7)=32;
   For n>7: a(n) = a(n-1) + a(n-4) - a(n-5) */

#include <stdlib.h>

/* Initial values for n = 0 … 7 */
static const seq_t init_vals[8] = {
    (seq_t)1, (seq_t)6, (seq_t)10, (seq_t)12,
    (seq_t)20, (seq_t)26, (seq_t)32, (seq_t)32
};

seq_t a(idx_t n)
{
    /* Static cache of computed values: indexed by n */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;      /* allocated capacity in elements   */
    static idx_t   next       = 0;      /* next index to be computed (exclusive) */

    /* Ensure enough space for n+1 entries */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = ((size_t)(n + 1)) * 2;
        if (new_cap == 0) new_cap = 1;   /* handle n == ~0ULL (unlikely) */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failed – signal error */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = next; i <= n; ++i) {
        if (i < 8) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = cache[i-1] + cache[i-4] - cache[i-5];
        }
    }
    next = n + 1;      /* indices < next are now initialized */

    return cache[n];
}
