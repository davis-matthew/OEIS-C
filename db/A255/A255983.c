/*
 * A255983  a(n) = 1 for n <= 5; a(n) = 36*a(n-1) - 450*a(n-2) + 3000*a(n-3) -
 *          11250*a(n-4) + 22500*a(n-5) - 18750*a(n-6) otherwise.
 * https://oeis.org/A255983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 for n<=5; a(n) = 36*a(n-1) - 450*a(n-2) + 3000*a(n-3) -
 *            11250*a(n-4) + 22500*a(n-5) - 18750*a(n-6) otherwise
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define CACHE_MAX 10001   /* supports n up to 10000 */

static seq_t cache[CACHE_MAX];
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Initialise the first 6 terms once */
    if (cache_size == 0) {
        for (idx_t i = 0; i <= 5; ++i) cache[i] = (seq_t)1;
        cache_size = 6;
    }

    /* Return a cached value if already computed */
    if (n < cache_size) return cache[n];

    /* Compute the sequence up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = 36 * cache[i-1]
                  - 450 * cache[i-2]
                  + 3000 * cache[i-3]
                  - 11250 * cache[i-4]
                  + 22500 * cache[i-5]
                  - 18750 * cache[i-6];
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
