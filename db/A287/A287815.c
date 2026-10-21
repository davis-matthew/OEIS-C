/*
 * A287815  Number of octonary sequences of length n such that no two
 *          consecutive terms have distance 7.
 * https://oeis.org/A287815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 6*a(n-2); a(0) = 1; a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization: cache[0]=1, cache[1]=8 */
    static seq_t *cache     = NULL;          /* array holding computed values   */
    static size_t cache_cap = 0;             /* allocated capacity              */
    static size_t cache_len = 0;             /* number of values stored (>=2)   */

    /* initial allocation: at least two elements for a(0) and a(1)   */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* out of memory                  */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache_len = 2;
    }

    /* grow cache capacity if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();              /* out of memory                */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute up to n if we haven't done so yet                */
    if (cache_len <= (size_t)n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(i) = 7*a(i-1) + 6*a(i-2)            */
            cache[(size_t)i] =
                (seq_t)7 * cache[(size_t)(i - 1)] +
                (seq_t)6 * cache[(size_t)(i - 2)];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
