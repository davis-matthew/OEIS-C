/*
 * A301792  Number of 3Xn 0..1 arrays with every element equal to 0 or 1
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=13, a(3)=12, a(4)=17, a(5)=25 a(n) = 2*a(n-1) -
 *            a(n-3) for n > 5 (indices are 1‑based as in OEIS)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache holds a(1) at cache[0], a(2) at cache[1], ...     */
    static seq_t *cache = NULL;
    static idx_t cacheSize = 0;

    if (n == 0)          /* undefined for the offset, return 0 */
        return (seq_t)0;

    /* enlarge the cache if necessary */
    if (n > cacheSize) {
        size_t needed = (size_t)n;
        seq_t *newCache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!newCache)   /* allocation failure – return 0 */
            return (seq_t)0;
        cache = newCache;

        /* compute missing values */
        for (idx_t i = cacheSize; i < n; ++i) {
            switch (i) {
                case 0: cache[i] = (seq_t)4;   break;   /* a(1) */
                case 1: cache[i] = (seq_t)13;  break;   /* a(2) */
                case 2: cache[i] = (seq_t)12;  break;   /* a(3) */
                case 3: cache[i] = (seq_t)17;  break;   /* a(4) */
                case 4: cache[i] = (seq_t)25;  break;   /* a(5) */
                default:
                    /* a(n) = 2*a(n-1) - a(n-3)  (i >= 5 corresponds to n >= 6) */
                    cache[i] = cache[i-1] + cache[i-1] - cache[i-3];
                    break;
            }
        }
        cacheSize = n;
    }

    return cache[n-1];
}
