/*
 * A263694  Expansion of (1 + x + x^2 + x^3 + 4*x^4 - x^5 - x^6 - x^7 +
 *          3*x^8)/((1 - x)^2*(1 + x + x^2 + x^3 + x^4 + x^5 + x^6 + x^7)).
 * https://oeis.org/A263694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A263694
    Recurrence:  a(n) = a(n-1) + a(n-8) – a(n-9)  (n ≥ 9)
    Initial values:  a(0)=1, a(1)=2, a(2)=3, a(3)=4, a(4)=8,
                     a(5)=7, a(6)=6, a(7)=5, a(8)=9
    The function accepts calls in any order and memoises the
    results.  For n ≥ 0 it returns the exact a(n).
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for indices 0 … 8 */
    static const seq_t base[9] = {1, 2, 3, 4, 8, 7, 6, 5, 9};

    /* cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of stored elements */

    /* trivial case: n within the initial window */
    if (n < 9)
        return base[(size_t)n];

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_len = 9;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (size_t i = 0; i < 9; ++i)
            cache[i] = base[i];
    }

    /* grow the cache if necessary, using realloc */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* compute the missing terms */
        for (idx_t cur = cache_len; cur <= n; ++cur) {
            seq_t val = cache[cur - 1] + cache[cur - 8] - cache[cur - 9];
            cache[cur] = val;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
