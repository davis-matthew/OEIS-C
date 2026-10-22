/*
 * A398051  Upper (1,1/5) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7) Initial
 *            values: a(0)=0, a(1)=2, a(2)=4, a(3)=8, a(4)=14, a(5)=20,
 *            a(6)=29
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len = 0;   /* number of stored terms */
    static size_t cap = 0;   /* capacity of cache */

    /* Initial allocation and base terms */
    if (cache == NULL) {
        cap = 8; /* at least enough for indices 0..7 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* fallback on allocation failure */

        cache[0] = (seq_t)0;   /* a(0) */
        cache[1] = (seq_t)2;   /* a(1) */
        cache[2] = (seq_t)4;   /* a(2) */
        cache[3] = (seq_t)8;   /* a(3) */
        cache[4] = (seq_t)14;  /* a(4) */
        cache[5] = (seq_t)20;  /* a(5) */
        cache[6] = (seq_t)29;  /* a(6) */
        len = 7;              /* we have stored indices 0..6 inclusive */
    }

    /* Grow cache if needed */
    if ((size_t)n >= len) {
        /* ensure capacity */
        if ((size_t)n >= cap) {
            size_t new_cap = cap;
            while ((size_t)n >= new_cap)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* fallback on allocation failure */
            cache = tmp;
            cap = new_cap;
        }

        /* Compute new terms up to n */
        for (idx_t i = (idx_t)len; i <= n; ++i) {
            seq_t an_1 = cache[(size_t)(i - 1)];
            seq_t an_2 = cache[(size_t)(i - 2)];
            seq_t an_5 = cache[(size_t)(i - 5)];
            seq_t an_6 = cache[(size_t)(i - 6)];
            seq_t an_7 = cache[(size_t)(i - 7)];
            seq_t val = (seq_t)2 * an_1 - an_2 + an_5 - (seq_t)2 * an_6 + an_7;
            cache[(size_t)i] = val;
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
