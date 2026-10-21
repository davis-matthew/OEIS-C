/*
 * A270608  Number of 3X3X3 triangular 0..n arrays with some element plus some
 *          adjacent element totalling n+1 or n-1 exactly once.
 * https://oeis.org/A270608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+3*a(n-2)-8*a(n-3)-2*a(n-4)+12*a(n-5) *
 *            -2*a(n-6)-8*a(n-7)+3*a(n-8)+2*a(n-9)-a(n-10) for n>17 * Initial
 *            values (n = 1 … 17): * 0, 0, 312, 1716, 8148, 23448, 67788,
 *            144252, 315936, * 568032, 1049532, 1692228, 2793240, 4176936,
 *            6369012, * 9014028, 12965136
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 49, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for the first 17 terms (1‑based index) */
    static const seq_t base[17] = {
        0ULL, 0ULL, 312ULL, 1716ULL, 8148ULL, 23448ULL, 67788ULL,
        144252ULL, 315936ULL, 568032ULL, 1049532ULL, 1692228ULL,
        2793240ULL, 4176936ULL, 6369012ULL, 9014028ULL, 12965136ULL
    };

    /* dynamic cache for all computed terms, 1‑based indexing adapted to 0‑based array */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* current allocated size (number of elements) */
    static idx_t computed = 0;     /* highest n that has been computed */

    /* initialise the cache on first call */
    if (cache == NULL) {
        cache_size = (17 > 0) ? 17 : 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)   /* allocation failure: swallow */
            return (seq_t)0;
        for (idx_t i = 0; i < 17; ++i) cache[i] = base[i];
        computed = 17;      /* we have computed up to n = 17 */
    }

    /* enlarge cache if required */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)   /* allocation failure: keep old cache */
            return (seq_t)0;
        cache = tmp;
        /* initialise newly allocated elements to 0 */
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* compute missing terms up to n (if any) */
    if (n > computed) {
        /* recurrence is valid for n > 17 */
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* only use recurrence for i > 17; for i <= 17 values are already known */
            if (i > 17) {
                cache[i-1] =
                    2 * cache[i-2] + 3 * cache[i-3] - 8 * cache[i-4] -
                    2 * cache[i-5] + 12 * cache[i-6] - 2 * cache[i-7] -
                    8 * cache[i-8] + 3 * cache[i-9] + 2 * cache[i-10] -
                     cache[i-11];
            }
        }
        computed = n;
    }

    /* a(1) is the first term; for n < 1 behaviour is unspecified */
    return (n >= 1) ? cache[n-1] : (seq_t)0;
}
