/*
 * A314994  Coordination sequence Gal.6.547.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-6) - a(n-7) for n > 10 with initial values
 *            a(0)=1, a(1)=5, a(2)=9, a(3)=15, a(4)=16, a(5)=24, a(6)=27,
 *            a(7)=34, a(8)=37, a(9)=44, a(10)=44
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    /* Initial values up to n = 10 */
    static const seq_t init[11] = {
        (seq_t)1,  (seq_t)5,  (seq_t)9,
        (seq_t)15, (seq_t)16, (seq_t)24,
        (seq_t)27, (seq_t)34, (seq_t)37,
        (seq_t)44, (seq_t)44
    };

    if (n >= cache_len) {
        /* Need to grow cache to hold indices up to n */
        idx_t old_len = cache_len;
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* In the unlikely event of allocation failure, abort.
               The problem statement does not require explicit error handling. */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        cache_len = n + 1;

        /* Fill new entries */
        for (idx_t i = old_len; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = init[i];
            } else {
                /* Recurrence: a(i) = a(i-1) + a(i-6) - a(i-7) */
                cache[i] = cache[i-1] + cache[i-6] - cache[i-7];
            }
        }
    }
    return cache[n];
}
