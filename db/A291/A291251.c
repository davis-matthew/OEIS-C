/*
 * A291251  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - 3 S + 2 S^3.
 * https://oeis.org/A291251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 6*a(n-2) - 2*a(n-3) - 6*a(n-4) + a(n-6) for n >= 7
 *            Initial values: a(0)=0, a(1)=3, a(2)=-2, a(3)=15, a(4)=-18,
 *            a(5)=76, a(6)=-126
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 24, i64 n <= 49, i128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial table for n < 7 */
    static const seq_t init_vals[7] = {
        (seq_t)0,
        (seq_t)3,
        (seq_t)-2,
        (seq_t)15,
        (seq_t)-18,
        (seq_t)76,
        (seq_t)-126
    };
    if (n < 7)
        return init_vals[(idx_t)n];

    /* Dynamic cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of elements stored (cache_len >= 7 after init) */

    if (cache_len == 0) {   /* first call */
        cache_len = 7;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = init_vals[i];
    }

    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));

        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            cache[i] = 6 * cache[i - 2]
                     - 2 * cache[i - 3]
                     - 6 * cache[i - 4]
                     +  cache[i - 6];
        }
        cache_len = new_len;
    }

    return cache[n];
}
