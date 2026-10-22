/*
 * A302512  Number of nX5 0..1 arrays with every element equal to 0, 1, 4 or 5
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-3) + 2*a(n-5) - 12*a(n-6) - 4*a(n-7) +
 *            8*a(n-9) for n > 11, with initial values a(1)=8, a(2)=7,
 *            a(3)=21, a(4)=36, a(5)=90, a(6)=184, a(7)=376, a(8)=832,
 *            a(9)=1744, a(10)=3684, a(11)=7908
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 57, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* array of computed a(i) */
    static idx_t cache_size = 0;     /* allocated size (index 0 unused) */
    static idx_t computed = 0;       /* highest index computed so far */

    if (n < 1)
        return (seq_t)0;            /* undefined for n < 1, return 0 */

    /* enlarge cache if necessary */
    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) /* out of memory */
            return (seq_t)0;
        cache = tmp;
        /* initialize new elements to 0 */
        memset((void *)(cache + cache_size), 0,
               (new_size - cache_size) * sizeof(seq_t));
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    if (computed < n) {
        /* 0‑index unused; we start filling at computed + 1 */
        static const seq_t init[12] = {
            0,            /* dummy for 0 */
            8, 7, 21, 36, 90, 184, 376, 832, 1744, 3684, 7908
        };
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i <= 11) {
                cache[i] = init[i];
            } else {
                /* recurrence: a(i) = a(i-1) + 6*a(i-3) + 2*a(i-5)
                                   - 12*a(i-6) - 4*a(i-7) + 8*a(i-9) */
                cache[i] = cache[i-1]
                           + (seq_t)6 * cache[i-3]
                           + (seq_t)2 * cache[i-5]
                           - (seq_t)12 * cache[i-6]
                           - (seq_t)4 * cache[i-7]
                           + (seq_t)8 * cache[i-9];
            }
        }
        computed = n;
    }

    return cache[n];
}
