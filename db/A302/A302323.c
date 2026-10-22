/*
 * A302323  Number of 2Xn 0..1 arrays with every element equal to 0, 1, 2 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) for n>4 with a(1)=2,
 *            a(2)=8, a(3)=20, a(4)=52
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t capacity = 0;

    if (n == 0)
        return (seq_t)0;                 /* undefined for n<1, safe fallback */

    /* initialize cache on first call */
    if (cache_len == 0) {
        capacity = (size_t)(n > 4 ? n + 1 : 5); /* enough to hold indices 1..4 at least */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* allocation failure, unlikely in tests */

        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)20;
        cache[4] = (seq_t)52;

        /* no need to initialize indices >4, they will be filled later */
        cache_len = 4;
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return cache[(size_t)n]; /* realloc failed; return whatever is available */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute new terms if required */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 4) {           /* already set */
            continue;
        }
        seq_t val = (seq_t)4 * cache[(size_t)(i - 1)]
                    - (seq_t)2 * cache[(size_t)(i - 2)]
                    - (seq_t)4 * cache[(size_t)(i - 3)];
        cache[(size_t)i] = val;
    }
    cache_len = n;
    return cache[(size_t)n];
}
