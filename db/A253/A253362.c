/*
 * A253362  Number of (n+2) X (3+2) 0..1 arrays with every 2 X 2 and 3 X 3
 *          subblock diagonal maximum minus antidiagonal minimum unequal to
 *          its neighbors horizontally and vertically.
 * https://oeis.org/A253362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 7*a(n-2) - 15*a(n-4) + 17*a(n-6) + 16*a(n-8) for n > 11
 *            * Initial values: * a(1) = 560, a(2) = 960, a(3) = 2992, a(4) =
 *            5280, a(5) = 13800, * a(6) = 27072, a(7) = 63496, a(8) = 130464,
 *            a(9) = 297288, * a(10) = 612480, a(11) =1410760, a(12) =2875104
 * Range:     u8 none, u16 n <= 7, u32 n <= 21, u64 n <= 49, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n < 1 */

    /* 0‑based cache, cache[0] is unused placeholder */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initial terms (index 1..12) */
    static const seq_t init[13] = {
        (seq_t)0,      /* placeholder for index 0 */
        (seq_t)560,
        (seq_t)960,
        (seq_t)2992,
        (seq_t)5280,
        (seq_t)13800,
        (seq_t)27072,
        (seq_t)63496,
        (seq_t)130464,
        (seq_t)297288,
        (seq_t)612480,
        (seq_t)1410760,
        (seq_t)2875104
    };

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = sizeof(init) / sizeof(init[0]);   /* 13 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;           /* allocation failed */
        memcpy(cache, init, cache_size * sizeof(seq_t));
    }

    /* grow cache if necessary and fill missing values using recurrence */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;            /* allocation failed */
        cache = tmp;
        /* compute terms from old cache_size up to n (inclusive) */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            /* recurrence uses only indices >= 1, which is guaranteed
             * because i >= 13 when this loop runs (cache_size is 13)
             */
            seq_t val =
                cache[i - 2] * (seq_t)7
                + cache[i - 6] * (seq_t)17
                + cache[i - 8] * (seq_t)16
                - cache[i - 4] * (seq_t)15;
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
