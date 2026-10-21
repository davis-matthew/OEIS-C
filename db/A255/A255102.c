/*
 * A255102  Number of length n+3 0..2 arrays with at most one downstep in
 *          every 3 consecutive neighbor pairs.
 * https://oeis.org/A255102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=3*a(n-1)-3*a(n-2)+8*a(n-3)-9*a(n-4)+3*a(n-5)-a(n-6) with
 *            a(1)=66, a(2)=168, a(3)=441, a(4)=1137, a(5)=2907, a(6)=7498
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 20, u64 n <= 43, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoized values, 1‑based index */
static idx_t  cache_len = 0;  /* largest index computed so far */

seq_t a(idx_t n)
{
    /* ensure the cache is large enough */
    if (n > cache_len) {
        /* reallocate to size n+1 (index 0 unused) */
        size_t new_size = (size_t)(n + 1) * sizeof(seq_t);
        cache = (seq_t *)realloc(cache, new_size);

        /* initialise the first six terms if not already present */
        static const seq_t init[6] = {66, 168, 441, 1137, 2907, 7498};
        idx_t i;
        for (i = 1; i <= 6 && i <= n; ++i) {
            if (i > cache_len)
                cache[i] = init[i - 1];
        }

        /* compute subsequent terms using the recurrence */
        for (i = (cache_len >= 6 ? cache_len + 1 : 7); i <= n; ++i) {
            seq_t sum = 3 * cache[i - 1] + 8 * cache[i - 3] + 3 * cache[i - 5];
            seq_t sub = 3 * cache[i - 2] + 9 * cache[i - 4] + cache[i - 6];
            cache[i] = sum - sub;
        }
        cache_len = n;
    }
    return cache[n];
}
