/*
 * A295672  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 1, a(1) = 1, a(2) =
 *          1, a(3) = -2.
 * https://oeis.org/A295672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), a(0)=1, a(1)=1, a(2)=1, a(3)=-2
 * Range:     i8 n <= 17, i16 n <= 28, i32 n <= 51, i64 n <= 98, i128 n <= 190,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Helper static cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_size = 0;   /* number of initialized entries */

    /* Handle base cases directly */
    if (n == 0 || n == 1 || n == 2)
        return 1;
    if (n == 3)
        return (seq_t)-2;

    /* Ensure the cache can hold index n */
    if (cache_size <= n) {
        /* Initial allocation if cache is empty */
        if (cache == NULL) {
            cache_cap = (size_t) ((n < 4) ? 4 : n + 1);
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) { /* allocation failed */ }
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)1;
            cache[3] = (seq_t)-2;
            cache_size = 4;
        }
        /* Extend capacity if necessary */
        else if (cache_cap < (size_t)(n + 1)) {
            size_t new_cap = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed */ }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t t1 = cache[(size_t)(i - 1)];
            seq_t t3 = cache[(size_t)(i - 3)];
            seq_t t4 = cache[(size_t)(i - 4)];
            cache[(size_t)i] = t1 + t3 + t4;
        }
        cache_size = n + 1;
    }

    return cache[(size_t)n];
}
