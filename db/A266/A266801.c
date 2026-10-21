/*
 * A266801  Coefficient of x^2 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A266801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*a(n-1) + 15*a(n-2) - 15*a(n-3) - 5*a(n-4) + a(n-5);
 *            a(0) = -7, a(1) = 23, a(2) = 65, a(3) = 653, a(4) = 3935
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Initialise cache with the first five values */
    if (!cache)
    {
        cache_cap = 8;               /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)-7;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)65;
        cache[3] = (seq_t)653;
        cache[4] = (seq_t)3935;
        cache_len = 5;
    }

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Expand cache capacity as needed */
    if (n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failed */
            return 0; /* undefined behaviour: caller must not request n */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Pre‑defined constants as seq_t */
    const seq_t c5  = (seq_t)5;
    const seq_t c15 = (seq_t)15;
    const seq_t nc5  = (seq_t)-5;
    const seq_t nc15 = (seq_t)-15;

    /* Iteratively compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t a0 = cache[i - 5];
        seq_t a1 = cache[i - 4];
        seq_t a2 = cache[i - 3];
        seq_t a3 = cache[i - 2];
        seq_t a4 = cache[i - 1];

        seq_t next = a0;                    /* a(n-5) */
        next += a1 * nc5;                   /* -5*a(n-4) */
        next += a2 * nc15;                  /* -15*a(n-3) */
        next += a3 * c15;                   /* +15*a(n-2) */
        next += a4 * c5;                    /* +5*a(n-1) */

        cache[i] = next;
    }

    cache_len = n + 1;
    return cache[n];
}
