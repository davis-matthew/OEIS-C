/*
 * A245135  Number of length 5 0..n arrays least squares fitting to a zero
 *          slope straight line, with a single point taken as having zero
 *          slope.
 * https://oeis.org/A245135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +2*a(n-2) -6*a(n-3) +6*a(n-5) -2*a(n-6)
 *            -2*a(n-7) +a(n-8) Initial values: a(1)=8, a(2)=39, a(3)=112,
 *            a(4)=275, a(5)=552, a(6)=1029, a(7)=1728, a(8)=2781
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* Static cache across calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initial values for n = 1..8 */
    static const seq_t base_vals[9] = {
        (seq_t)0,          /* unused index 0 */
        (seq_t)8,          /* a(1) */
        (seq_t)39,         /* a(2) */
        (seq_t)112,        /* a(3) */
        (seq_t)275,        /* a(4) */
        (seq_t)552,        /* a(5) */
        (seq_t)1029,       /* a(6) */
        (seq_t)1728,       /* a(7) */
        (seq_t)2781        /* a(8) */
    };

    /* Initialise the cache once */
    if (cache_len == 0) {
        size_t init_len = 8;
        cache = (seq_t *)malloc((init_len + 1) * sizeof(seq_t));
        if (!cache) abort();
        /* cache[0] unused, keep as 0 */
        for (size_t i = 1; i <= init_len; ++i)
            cache[i] = base_vals[i];
        cache_len = init_len;
    }

    /* Extend the cache if the requested n is larger */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = cache_len + 1; i <= new_len; ++i) {
            seq_t val = (seq_t)2 * cache[i - 1] +
                        (seq_t)2 * cache[i - 2] -
                        (seq_t)6 * cache[i - 3] +
                        (seq_t)6 * cache[i - 5] -
                        (seq_t)2 * cache[i - 6] -
                        (seq_t)2 * cache[i - 7] +
                        cache[i - 8];
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
