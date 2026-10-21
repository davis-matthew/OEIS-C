/*
 * A291397  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - S - S^3.
 * https://oeis.org/A291397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+3*a(n-4)+3*a(n-5)+a(n-6) for n >= 7
 *            with initial values a(0)=1, a(1)=2, a(2)=4, a(3)=10, a(4)=22,
 *            a(5)=46, a(6)=97
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;    /* memoisation array */
    static size_t cap   = 0;       /* current allocated size (in elements) */
    static idx_t max_computed = 0; /* largest n already computed */

    /* initialise on first call */
    if (cache == NULL) {
        /* reserve at least 8 entries to hold base values (0..6) */
        cap = 8;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;   /* allocation failure */

        /* set the known initial terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)22;
        cache[5] = (seq_t)46;
        cache[6] = (seq_t)97;
        max_computed = 6;
    }

    /* if the request is already satisfied, return it immediately */
    if (n <= max_computed)
        return cache[(size_t)n];

    /* grow the cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute new values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v = cache[(size_t)(i - 1)] +
                  cache[(size_t)(i - 2)] +
                  cache[(size_t)(i - 3)] +
                  (seq_t)3 * cache[(size_t)(i - 4)] +
                  (seq_t)3 * cache[(size_t)(i - 5)] +
                  cache[(size_t)(i - 6)];
        cache[(size_t)i] = v;
    }
    max_computed = n;
    return cache[(size_t)n];
}
