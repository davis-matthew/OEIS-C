/*
 * A351707  G.f. A(x) satisfies: A(x) = 1 + x + x^2 + x^3 + x^4 * A(x/(1 - x))
 *          / (1 - x)^2.
 * https://oeis.org/A351707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for 0 ≤ n ≤ 3; a(n) = Σ_{k=0}^{n-4} binomial(n-3, k+1)
 *            * a(k) for n ≥ 4
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last = 3; /* last index that has been computed */

    /* initialize cache on first call if needed */
    if (cache == NULL) {
        /* allocate at least up to n+1 or 5 */
        size_t init_cap = (size_t)n + 1;
        if (init_cap < 5) init_cap = 5;
        cache = (seq_t *)realloc(NULL, init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        /* set initial terms */
        for (idx_t i = 0; i <= 3; ++i)
            cache[i] = (seq_t)1;
        last = 3;
    }

    /* return cached value if already computed */
    if (n <= (idx_t)last)
        return cache[n];

    /* ensure cache capacity */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0; /* allocation failure, unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* compute a(i) using the recurrence */
        seq_t sum = 0;
        seq_t C = 1;                     /* binomial(i-3, 0) */
        idx_t up = i - 3;                 /* largest j index */
        for (idx_t j = 1; j <= up; ++j) {
            /* update binomial coefficient C = C * (i-2 - j) / j */
            C = C * (seq_t)(i - 2 - j);
            C = C / (seq_t)j;
            /* add term C * a(j-1) */
            sum += C * cache[j - 1];
        }
        cache[i] = sum;
    }

    last = n;
    return cache[n];
}
