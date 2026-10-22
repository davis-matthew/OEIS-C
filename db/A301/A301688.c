/*
 * A301688  Coordination sequence for node of type V2 in "krh" 2-D tiling (or
 *          net).
 * https://oeis.org/A301688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) - a(n-6) for n ≥ 7 Initial values (n = 0 … 6):
 *            a(0)=1, a(1)=4, a(2)=9, a(3)=12, a(4)=17, a(5)=22, a(6)=24 (The
 *            recurrence includes a constant term for n=6 because of the
 *            numerator of the generating function. For n ≥ 7 the constant
 *            term vanishes, giving the homogeneous recurrence a...
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n) {
    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;               /* number of cached values */

    /* Save constant init values */
    static const seq_t init_vals[7] = {1, 4, 9, 12, 17, 22, 24};

    /* Expand cache if necessary */
    if (n >= cache_len) {
        /* Allocate or reallocate the array */
        size_t new_len = (size_t)(n + 1); /* size_t needed for malloc */
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory – abort */
            abort();
        }
        cache = new_cache;

        /* Compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;
            if (i <= 6) {                   /* Initial block */
                val = init_vals[i];
            } else {                        /* Recurrence: a(i) = 2*a(i-3) - a(i-6) */
                /* The recurrence is valid for all i ≥ 7 */
                val = 2 * cache[i - 3] - cache[i - 6];
            }
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
