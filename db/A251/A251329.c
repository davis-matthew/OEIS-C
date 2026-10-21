/*
 * A251329  Number of (n+1) X (2+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 2 4 or 6.
 * https://oeis.org/A251329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 98*a(n-2) + 100*a(n-3) + 331*a(n-4) * +
 *            42*a(n-5) - 72*a(n-6) * Initial terms: a(1)=171, a(2)=1115,
 *            a(3)=7393, a(4)=49937, * a(5)=343299, a(6)=2399159
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based cache; cache[0] unused */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;  /* largest computed index */

    if (n < 1) return (seq_t)0;

    /* initialise cache with the first six terms on first call */
    if (cache == NULL) {
        cache_size = 6;
        cache = (seq_t*)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[1] = (seq_t)171;
        cache[2] = (seq_t)1115;
        cache[3] = (seq_t)7393;
        cache[4] = (seq_t)49937;
        cache[5] = (seq_t)343299;
        cache[6] = (seq_t)2399159;
    }

    /* If more terms are required, extend allocation first */
    if (n > cache_size) {
        idx_t new_size = cache_size * 2;
        if (new_size < n) new_size = n;
        seq_t *tmp = (seq_t*)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        /* cache_size remains old until we compute new entries */
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t pos = cache[i-1];
        pos *= (seq_t)18;
        seq_t tmp = cache[i-3];
        tmp *= (seq_t)100;
        pos += tmp;
        tmp = cache[i-4];
        tmp *= (seq_t)331;
        pos += tmp;
        tmp = cache[i-5];
        tmp *= (seq_t)42;
        pos += tmp;

        seq_t neg = cache[i-2];
        neg *= (seq_t)98;
        tmp = cache[i-6];
        tmp *= (seq_t)72;
        neg += tmp;

        cache[i] = pos - neg;
    }

    /* Update cache size to reflect the highest computed index */
    if (cache_size < n) cache_size = n;

    return cache[n];
}
