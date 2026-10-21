/*
 * A265804  Coefficient of x^2 in minimal polynomial of the continued fraction
 *          [1^n,5,1,1,1,...], where 1^n means n ones.
 * https://oeis.org/A265804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3); a(0)=1, a(1)=19, a(2)=29
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 27, u128 n <= 27,
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
    static idx_t last_computed = 2; /* index of the last precomputed term */

    /* base constants */
    const seq_t c1  = (seq_t)1;
    const seq_t c19 = (seq_t)19;
    const seq_t c29 = (seq_t)29;

    if (cache == NULL) {
        /* allocate at least enough space for (n+1) elements */
        cache_size = (idx_t)((size_t)n + 1);
        if (cache_size < 3) cache_size = 3;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = c1;
        cache[1] = c19;
        cache[2] = c29;
        last_computed = 2;
        if (n <= 2) return cache[n];
    }

    if (n <= last_computed)
        return cache[n];

    /* grow cache if needed */
    if (n + 1 > cache_size) {
        idx_t new_size = cache_size;
        while (n + 1 > new_size) new_size <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = cache[i-1] << 1;        /* 2*a(i-1) */
        val += cache[i-2] << 1;            /* +2*a(i-2) */
        val -= cache[i-3];                 /* -a(i-3) */
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
