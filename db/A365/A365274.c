/*
 * A365274  a(n) = a(n-2) + 4*a(n-4) - 2*a(n-8) - a(n-10), with a[0..9] = [1,
 *          1, 1, 2, 3, 5, 7, 13, 18, 31].
 * https://oeis.org/A365274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 4*a(n-4) - 2*a(n-8) - a(n-10), a(0..9) = [1, 1,
 *            1, 2, 3, 5, 7, 13, 18, 31]
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Initial values of the sequence */
static const seq_t init_vals[10] = {
    (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3,
    (seq_t)5, (seq_t)7, (seq_t)13, (seq_t)18, (seq_t)31
};

/* Cached values: cache[0] … cache[cache_sz-1] */
static seq_t *cache      = NULL;   /* array of computed values   */
static idx_t cache_sz = 0;        /* number of allocated slots  */
static idx_t last_computed = 0;   /* highest index that has been filled */

/* Ensure the internal array is large enough for indices 0..n */
static void ensure_cache(idx_t n)
{
    if (cache_sz <= n) {
        /* realloc returns void* in C++: cast to the proper type */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        /* In a real implementation, you would check tmp for NULL. */
        cache = tmp;
        cache_sz = n + 1;
    }
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    /* Directly return the first ten values. */
    if (n < 10) {
        return init_vals[n];
    }

    /* Grow the cache if necessary. */
    ensure_cache(n);

    /* Initialize the cached base terms if they are not already in place. */
    if (last_computed < 9) {
        for (idx_t i = 0; i <= 9; ++i) {
            cache[i] = init_vals[i];
        }
        last_computed = 9;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* a(i) = a(i-2) + 4*a(i-4) - 2*a(i-8) - a(i-10) */
        cache[i] = cache[i - 2]
                 + (seq_t)4 * cache[i - 4]
                 - (seq_t)2 * cache[i - 8]
                 - cache[i - 10];
    }
    last_computed = n;

    return cache[n];
}
