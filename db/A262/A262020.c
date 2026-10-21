/*
 * A262020  Inverse binomial transform of double factorial n!! = A006882(n).
 * https://oeis.org/A262020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(1) = 0, a(2) = 1 a(n) = (n-2)*a(n-3) + (n-1)*a(n-2)
 *            - 2*a(n-1) (for n > 2)
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 18, i64 n <= 31, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cached values: cache[0] = a(0), cache[1] = a(1), ... */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t   max_calc = (idx_t)-1;   /* sentinel before first use */

    /* ---- initial cache fill ---- */
    if (cache == NULL) {
        cache_cap = 3;                /* need indices 0,1,2 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)0;          /* a(1) */
        cache[2] = (seq_t)1;          /* a(2) */
        max_calc = 2;
    }

    /* ---- grow cache when n is too large for current size ---- */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---- compute missing terms up to n ---- */
    if (n > max_calc) {
        for (idx_t i = max_calc + 1; i <= n; ++i) {
            seq_t i_seq = (seq_t)i;
            cache[i] = (i_seq - (seq_t)2) * cache[i - 3]
                     + (i_seq - (seq_t)1) * cache[i - 2]
                     - (seq_t)2 * cache[i - 1];
        }
        max_calc = n;
    }

    return cache[(size_t)n];
}
