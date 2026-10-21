/*
 * A252152  Number of (n+2)X(3+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum not equal to 0 1 3 6 or 7 and every 3X3 diagonal and
 *          antidiagonal sum equal to 0 1 3 6 or 7.
 * https://oeis.org/A252152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 2*a(n-3) - 6*a(n-4) - 24*a(n-5) +
 *            16*a(n-6) + 8*a(n-7) + 32*a(n-8) - 32*a(n-9) for n > 14 Initial
 *            values (a(1) … a(14)): 2591, 1898, 3197, 4988, 9572, 17803,
 *            35924, 68613, 140294, 268864, 553258, 1064524, 2194400, 4229940
 * Range:     u8 none, u16 n <= 7, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_values[15] = {
    0,
    2591,
    1898,
    3197,
    4988,
    9572,
    17803,
    35924,
    68613,
    140294,
    268864,
    553258,
    1064524,
    2194400,
    4229940
};

seq_t a(idx_t n)
{
    /* Cache of computed values – memoized across calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* highest index stored (0 if none) */

    /* Initialise cache with known terms up to 14 on first call */
    if (cache_size == 0) {
        cache_size = 14;
        cache = (seq_t *)calloc(cache_size + 1, sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
        for (idx_t i = 0; i <= cache_size; ++i)
            cache[i] = init_values[i];
    }

    if (n <= cache_size)
        return cache[n];

    /* Expand cache to accommodate n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)
        return (seq_t)0; /* allocation failure */
    cache = new_cache;

    /* Compute new terms using the empirical linear recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val =
            cache[i - 1]
            + (seq_t)4 * cache[i - 2]
            + (seq_t)2 * cache[i - 3]
            - (seq_t)6 * cache[i - 4]
            - (seq_t)24 * cache[i - 5]
            + (seq_t)16 * cache[i - 6]
            + (seq_t)8  * cache[i - 7]
            + (seq_t)32 * cache[i - 8]
            - (seq_t)32 * cache[i - 9];
        cache[i] = val;
    }
    cache_size = n;
    return cache[n];
}
