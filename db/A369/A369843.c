/*
 * A369843  Number of compositions of 5*n-3 into parts 2 and 5.
 * https://oeis.org/A369843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) with
 *            initial values a(1)=1, a(2)=2, a(3)=4, a(4)=11, a(5)=34. This
 *            recurrence yields the number of compositions of 5*n-3 into parts
 *            2 and 5 (OEIS A369843)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to support arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t max_calc = 0;

    if (n < 1) return 0;          /* undefined for n=0, but not used */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate space for indices 0..5 (0 unused) */
        cache = (seq_t *)malloc(sizeof(seq_t) * (5 + 1));
        if (!cache) return 0;    /* allocation failed */
        for (idx_t i = 0; i <= 5; ++i) cache[i] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 4;
        cache[4] = 11;
        cache[5] = 34;
        max_calc = 5;
    }

    if (n > max_calc) {
        /* enlarge cache to accommodate up to index n */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) return 0;   /* allocation failed */
        cache = new_cache;

        /* compute new terms using recurrence */
        for (idx_t i = max_calc + 1; i <= n; ++i) {
            cache[i] = (seq_t)5 * cache[i-1]
                     - (seq_t)9 * cache[i-2]
                     + (seq_t)10 * cache[i-3]
                     - (seq_t)5 * cache[i-4]
                     + cache[i-5];
        }
        max_calc = n;
    }

    return cache[n];
}
