/*
 * A252008  Number of (n+2)X(1+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum 1 3 6 or 8 and every diagonal and antidiagonal sum not
 *          1 3 6 or 8.
 * https://oeis.org/A252008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) -2*a(n-2) +5*a(n-3) -8*a(n-4) +9*a(n-5)
 *            -8*a(n-6) * +6*a(n-7) -9*a(n-8) +6*a(n-9) +a(n-10) -a(n-11)
 *            -3*a(n-13) +3*a(n-14) * Initial terms (n = 1 … 22): * 756, 1168,
 *            1110, 1618, 1970, 2458, 3246, 4410, 5802, 7874, *
 *            10910,14594,19726,27894,38154,52258,74678,102670,141578,205578,
 *            *...
 * Range:     u8 none, u16 n <= 16, u32 n <= 47, u64 n <= 108, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init_seq[23] = {
    0,          /* unused index 0 */
    756, 1168, 1110, 1618, 1970, 2458, 3246, 4410, 5802, 7874,
    10910, 14594, 19726, 27894, 38154, 52258, 74678, 102670,
    141578, 205578, 285918, 396882
};

seq_t a(idx_t n)
{
    if (n <= 22)
        return _init_seq[n];

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t computed_until = 22;   /* largest n that has been computed */
    static idx_t cache_alloc   = 23;    /* actual size allocated (indexes 0 … cache_alloc-1) */

    if (!cache) {
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        /* initialise first 22 values and a dummy zero at index 0 */
        for (idx_t i = 1; i <= 22; ++i)
            cache[i] = _init_seq[i];
        cache[0] = 0;
    }

    if (n >= cache_alloc) {
        /* grow the cache */
        cache_alloc = n + 1;
        cache = (seq_t *)realloc(cache, cache_alloc * sizeof(seq_t));
    }

    if (n > computed_until) {
        /* compute missing terms using the recurrence */
        for (idx_t i = computed_until + 1; i <= n; ++i) {
            seq_t pos = 0, neg = 0;
            /* positive contributions */
            pos +=  2 * cache[i-1];
            pos +=  5 * cache[i-3];
            pos +=  9 * cache[i-5];
            pos +=  6 * cache[i-7];
            pos +=  6 * cache[i-9];
            pos +=     cache[i-10];
            pos +=  3 * cache[i-14];
            /* negative contributions */
            neg +=  2 * cache[i-2];
            neg +=  8 * cache[i-4];
            neg +=  8 * cache[i-6];
            neg +=  9 * cache[i-8];
            neg +=     cache[i-11];
            neg +=  3 * cache[i-13];
            /* final value (guaranteed non‑negative during subtraction) */
            cache[i] = pos - neg;
        }
        computed_until = n;
    }

    return cache[n];
}
