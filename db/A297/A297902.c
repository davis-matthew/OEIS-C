/*
 * A297902  Number of nX3 0..1 arrays with every element equal to 0, 1, 3 or 4
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) =
 *            a(n-1)+2*a(n-2)+5*a(n-3)-4*a(n-4)-7*a(n-5)-6*a(n-6)
 *            +5*a(n-7)+4*a(n-8)+a(n-9)-2*a(n-10)+a(n-11)+a(n-12) for n>13
 *            Initial values:
 *            a(1)=3,a(2)=3,a(3)=1,a(4)=4,a(5)=5,a(6)=5,a(7)=20,a(8)=29,
 *            a(9)=63,a(10)=156,a(11)=311,a(12)=691,a(13)=1568
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 31, u64 n <= 59, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t cached_n_max = 0;   /* largest n for which the cache is filled */

    if (n == 0)   /* safe guard – sequence is defined only for n>=1 */
        return 0;

    /* Ensure base values exist in the cache */
    if (cached_n_max < 13) {
        const size_t need = 13;
        if (cap < need) {
            seq_t *new_cache = (seq_t*)realloc(cap ? cache : NULL,
                                               need * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
            cap   = need;
        }
        const seq_t base[] = { (seq_t)3, (seq_t)3, (seq_t)1,
                               (seq_t)4, (seq_t)5, (seq_t)5,
                               (seq_t)20, (seq_t)29, (seq_t)63,
                               (seq_t)156, (seq_t)311, (seq_t)691,
                               (seq_t)1568 };
        for (size_t i = 0; i < need; ++i)
            cache[i] = base[i];
        cached_n_max = 13;
    }

    /* Return cached value if already known */
    if (n <= cached_n_max)
        return cache[n-1];

    /* Grow the cache to accommodate up to n */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t*)realloc(cache,
                                           new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute new terms using the recurrence */
    for (idx_t i = cached_n_max + 1; i <= n; ++i) {
        size_t j = (size_t)(i - 1);                 /* cache index for a(i) */
        cache[j] =
            cache[j-1]
          + (seq_t)2 * cache[j-2]
          + (seq_t)5 * cache[j-3]
          - (seq_t)4 * cache[j-4]
          - (seq_t)7 * cache[j-5]
          - (seq_t)6 * cache[j-6]
          + (seq_t)5 * cache[j-7]
          + (seq_t)4 * cache[j-8]
          + cache[j-9]
          - (seq_t)2 * cache[j-10]
          + cache[j-11]
          + cache[j-12];
    }
    cached_n_max = n;
    return cache[n-1];
}
