/*
 * A304770  Number of n X 3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-3) + 2*a(n-4) - a(n-5) for n > 8
 *            with initial values: a(1)=4, a(2)=21, a(3)=27, a(4)=67,
 *            a(5)=139, a(6)=303, a(7)=685, a(8)=1511 (OEIS A304770)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 54, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t   cached_max = 0;   /* highest n computed so far */
    static size_t  cache_cap  = 0;   /* allocated capacity (index range 1..cache_cap) */

    /* initialisation on first call */
    if (cache == NULL) {
        cache_cap = 16;                     /* arbitrary start size */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure – unlikely */
        /* fill the known initial terms */
        cache[1] = 4;
        cache[2] = 21;
        cache[3] = 27;
        cache[4] = 67;
        cache[5] = 139;
        cache[6] = 303;
        cache[7] = 685;
        cache[8] = 1511;
        cached_max = 8;
    }

    /* extend capacity if needed */
    if (n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap = new_cap * 2 + 8;           /* grow by factor 2 + 8 */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n if not already done */
    if (n > cached_max) {
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i <= 8) {
                /* already filled initial terms – do nothing */
                continue;
            }
            cache[i] =
                cache[i - 1] + cache[i - 2] +
                (seq_t)3 * cache[i - 3] +
                (seq_t)2 * cache[i - 4] -
                cache[i - 5];
        }
        cached_max = n;
    }

    return cache[n];
}
