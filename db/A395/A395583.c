/*
 * A395583  a(n) = floor(r*a(n-1)) + floor(r*a(n-2)), a(0) = 0, a(1) = 1 and r
 *          = 2 + sqrt(5).
 * https://oeis.org/A395583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + a(n-2) - 3*a(n-3) - a(n-4) - a(n-5) Initial
 *            values: a(0)=0, a(1)=1, a(2)=4, a(3)=20, a(4)=100
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* array of already computed terms */
    static size_t cache_size = 0; /* allocated length of cache */
    static idx_t computed = 0;    /* number of terms stored (valid indices 0..computed-1) */

    /* grow the cache if necessary */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */

        if (cache) {
            memcpy(tmp, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* compute new terms if they are missing */
    if (n + 1 > computed) {
        for (idx_t i = computed; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 0;
            } else if (i == 1) {
                cache[i] = 1;
            } else if (i == 2) {
                cache[i] = 4;
            } else if (i == 3) {
                cache[i] = 20;
            } else if (i == 4) {
                cache[i] = 100;
            } else {
                /* recurrence: a(n) = 5*a(n-1) + a(n-2) - 3*a(n-3) - a(n-4) - a(n-5) */
                seq_t t1 = 5 * cache[i - 1] + cache[i - 2];
                seq_t t2 = 3 * cache[i - 3] + cache[i - 4] + cache[i - 5];
                cache[i] = t1 - t2; /* `t1` is guaranteed to be ≥ `t2` for all n≥5 */
            }
        }
        computed = n + 1;
    }

    return cache[n];
}
