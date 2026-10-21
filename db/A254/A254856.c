/*
 * A254856  Indices of centered heptagonal numbers (A069099) that are also
 *          octagonal numbers (A000567).
 * https://oeis.org/A254856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+26*a(n-2)-26*a(n-3)-a(n-4)+a(n-5) with initial
 *            values a(1)=1, a(2)=2, a(3)=15, a(4)=40, a(5)=377
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation (zero‑based index unused) */
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;        /* highest index computed */

    /* handle small indices directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)15;
    if (n == 4) return (seq_t)40;
    if (n == 5) return (seq_t)377;

    /* initialise cache */
    if (cache == NULL) {
        cache = (seq_t *)malloc((6) * sizeof(seq_t));   /* indices 0..5 */
        if (cache == NULL) return 0;                     /* out of memory */
        cache[0] = 0;                                   /* unused */
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 15;
        cache[4] = 40;
        cache[5] = 377;
        cache_n  = 5;
    }

    /* extend cache if necessary */
    if (n > cache_n) {
        idx_t new_size = n + 1;            /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;         /* out of memory */
        cache = tmp;
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            /* recurrence: a(i) = a(i-1) + 26*a(i-2) - 26*a(i-3) - a(i-4) + a(i-5) */
            cache[i] = cache[i-1]
                     + (seq_t)26 * cache[i-2]
                     - (seq_t)26 * cache[i-3]
                     - cache[i-4]
                     + cache[i-5];
        }
        cache_n = n;
    }

    return cache[n];
}
