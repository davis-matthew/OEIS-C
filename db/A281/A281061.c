/*
 * A281061  Number of 6Xn 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -12*a(n-2) +5*a(n-3) +12*a(n-4) -12*a(n-5)
 *            -3*a(n-6) +6*a(n-7) - a(n-9) for n > 19 Initial values (n = 1 …
 *            19): 0, 468, 4988, 10112, 22106, 46610, 87995, 159317, 282552,
 *            495655, 864369, 1501627, 2601791, 4498333, 7762266, 13369326,
 *            22983871, 39440039, 67556367
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 26, u64 n <= 70, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* first 19 terms – stored as an initializer for quick return */
    static const seq_t init[19] = {
        0ULL, 468ULL, 4988ULL, 10112ULL, 22106ULL,
        46610ULL, 87995ULL, 159317ULL, 282552ULL,
        495655ULL, 864369ULL, 1501627ULL, 2601791ULL,
        4498333ULL, 7762266ULL, 13369326ULL, 22983871ULL,
        39440039ULL, 67556367ULL
    };

    /* n <= 19 – return immediately, no memoisation needed */
    if (n <= 19)
        return init[n - 1];

    /* dynamic cache for terms that have been computed */
    static seq_t *cache = NULL;          /* array of terms 0 … current_max */
    static idx_t  cache_max = 0;          /* highest index that has a value */
    static size_t cache_size = 0;         /* allocated size of cache array */

    /* ensure the array is large enough for indices 0 … n */
    size_t need = (size_t)(n + 1);
    if (cache == NULL) {
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        cache_size = need;
    } else if (cache_size < need) {
        cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        cache_size = need;
    }

    /* initialise the first 19 terms in the cache once */
    if (cache_max < 19) {
        for (idx_t i = 1; i <= 19; ++i)
            cache[i] = init[i - 1];
        cache_max = 19;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        pos += (seq_t)6 * cache[i - 1];
        pos += (seq_t)5 * cache[i - 3];
        pos += (seq_t)12 * cache[i - 4];
        pos += (seq_t)6 * cache[i - 7];

        neg += (seq_t)12 * cache[i - 2];
        neg += (seq_t)12 * cache[i - 5];
        neg += (seq_t)3  * cache[i - 6];
        neg += (seq_t)1  * cache[i - 9];

        cache[i] = pos - neg;           /* safe since pos >= neg for all n > 19 */
    }

    cache_max = n;
    return cache[n];
}
