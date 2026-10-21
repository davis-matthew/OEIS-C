/*
 * A250144  Number of length 5+2 0..n arrays with the medians of every three
 *          consecutive terms nondecreasing.
 * https://oeis.org/A250144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) * +
 *            56*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8) for n > 8 * Initial
 *            values (offset 1): * a(1)=68, a(2)=900, a(3)=5696, a(4)=24125,
 *            a(5)=79200, * a(6)=217856, a(7)=526032, a(8)=1149057
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 29, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Handle impossible or trivial indices */
    if (n == 0) return 0; /* offset is 1, a(0) is undefined */

    /* Ensure cache covers indices up to n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;                     /* indices: 0 .. n */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        if (!new_cache) return 0; /* allocation failed */

        /* copy old contents if any */
        if (cache) {
            for (idx_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
        }

        /* Initial values up to index 8 */
        static const seq_t init[9] = {
            0,          /* dummy for index 0 */
            68,         /* a(1) */
            900,        /* a(2) */
            5696,       /* a(3) */
            24125,      /* a(4) */
            79200,      /* a(5) */
            217856,     /* a(6) */
            526032,     /* a(7) */
            1149057     /* a(8) */
        };

        /* Compute missing terms */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i <= 8) {
                new_cache[i] = init[i];
            } else {
                /* recurrence */
                new_cache[i] =
                    (seq_t)8 * new_cache[i - 1]
                    - (seq_t)28 * new_cache[i - 2]
                    + (seq_t)56 * new_cache[i - 3]
                    - (seq_t)70 * new_cache[i - 4]
                    + (seq_t)56 * new_cache[i - 5]
                    - (seq_t)28 * new_cache[i - 6]
                    + (seq_t)8  * new_cache[i - 7]
                    - new_cache[i - 8];
            }
        }

        /* Replace old cache */
        if (cache) free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
