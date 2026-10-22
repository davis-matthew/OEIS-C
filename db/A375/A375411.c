/*
 * A375411  Expansion of e.g.f. exp(-x * (1 - x)^3) / (1 - x).
 * https://oeis.org/A375411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-1)*a(n-1) + 7*(n-1)*a(n-2) - 15*(n-1)*(n-2)*a(n-3) +
 *            13*(n-1)*(n-2)*(n-3)*a(n-4) - 4*(n-1)*(n-2)*(n-3)*(n-4)*a(n-5),
 *            with initial values a(0)=1, a(1)=0, a(2)=7, a(3)=-16, a(4)=177.
 *            The algorithm uses a cached transformation based on this
 *            recurrence
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with first five terms */
    if (cache == NULL) {
        cache_size = 5;                     /* cache[0]..cache[4] */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 7;
        cache[3] = -16;
        cache[4] = 177;
    }

    /* already computed? */
    if (n < cache_size) {
        return cache[n];
    }

    /* grow cache to accommodate index n */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        return 0;   /* allocation failure – caller will handle */
    }
    cache = tmp;

    /* compute new values */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t n1 = (seq_t)(i - 1);
        seq_t n2 = (seq_t)(i - 2);
        seq_t n3 = (seq_t)(i - 3);
        seq_t n4 = (seq_t)(i - 4);

        seq_t term1 = n1 * cache[i - 1];
        seq_t term2 = 7 * n1 * cache[i - 2];
        seq_t term3 = -15 * n1 * n2 * cache[i - 3];
        seq_t term4 = 13 * n1 * n2 * n3 * cache[i - 4];
        seq_t term5 = -4 * n1 * n2 * n3 * n4 * cache[i - 5];

        cache[i] = term1 + term2 + term3 + term4 + term5;
    }

    cache_size = new_cap;
    return cache[n];
}
