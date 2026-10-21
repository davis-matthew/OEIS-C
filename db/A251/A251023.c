/*
 * A251023  Number of (n+1)X(3+1) 0..3 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements greater than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1)
 *            -25*a(n-2)+35*a(n-3)-7*a(n-4)-49*a(n-5)+77*a(n-6)
 *            -55*a(n-7)+20*a(n-8)-3*a(n-9) with the initial values:
 *            a(1)=1966, a(2)=8207, a(3)=20608, a(4)=51347, a(5)=118871,
 *            a(6)=272547, a(7)=608267, a(8)=1356939, a(9)=3017800,
 *            a(10)=6755296, a(11)=15194024, a(12)=34402203,
 *            a(13)=78257659,...
 * Range:     u8 none, u16 n <= 4, u32 n <= 17, u64 n <= 44, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Base values for n = 1 … 14 */
    static const seq_t base[15] = {
        0,          /* a(0) unused */
        1966,
        8207,
        20608,
        51347,
        118871,
        272547,
        608267,
        1356939,
        3017800,
        6755296,
        15194024,
        34402203,
        78257659,
        178761713
    };

    /* Return the known value for n ≤ 14 */
    if (n <= 14) {
        return base[n];
    }

    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* allocated size (indexes 0 .. cache_len-1) */
    static idx_t max_computed = 0;   /* largest n for which cache[n] is valid */

    /* Initialise the cache with the base values (once) */
    if (max_computed == 0) {
        cache_len = 15;                     /* indices 0 … 14 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failed, return 0 as sentinel */
        for (idx_t i = 0; i < 15; ++i)
            cache[i] = base[i];
        max_computed = 14;
    }

    /* Return cached value if already computed */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Ensure the cache array is large enough for the requested n */
    if (n + 1 > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failed */
        cache = tmp;
        cache_len = n + 1;
    }

    /* Compute missing terms using the 9‑term linear recurrence */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t pos = 8 * cache[k - 1]
                  + 35 * cache[k - 3]
                  + 77 * cache[k - 6]
                  + 20 * cache[k - 8];
        seq_t neg = 25 * cache[k - 2]
                  + 7  * cache[k - 4]
                  + 49 * cache[k - 5]
                  + 55 * cache[k - 7]
                  + 3  * cache[k - 9];
        cache[k] = pos - neg;
    }

    max_computed = n;
    return cache[n];
}
