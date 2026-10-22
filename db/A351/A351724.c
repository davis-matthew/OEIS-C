/*
 * A351724  Number of compositions of n into parts of size 1, 5, 10 or 25.
 * https://oeis.org/A351724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-5) + a(n-10) + a(n-25), a(0)=1
 * Range:     u8 n <= 20, u16 n <= 38, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization across calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if ((idx_t)(n + 1) > cache_size) {
        /* need to grow cache */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) {          /* allocation failure – unlikely in tests */
            return 0;
        }
        cache = new_cache;

        idx_t prev_size = cache_size;
        if (prev_size == 0) {      /* first time: set the base case */
            cache[0] = (seq_t)1;
            prev_size = 1;
        }

        for (idx_t i = prev_size; i <= n; ++i) {
            seq_t val = cache[i - 1];
            if (i >= 5)  val += cache[i - 5];
            if (i >= 10) val += cache[i - 10];
            if (i >= 25) val += cache[i - 25];
            cache[i] = val;
        }

        cache_size = n + 1;
    }

    return cache[n];
}
