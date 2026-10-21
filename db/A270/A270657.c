/*
 * A270657  a(3n)=a(n), a(3n+1)=a(3(n+1))-a(3n), a(3n+2)=a(3n)+a(3(n+1));
 *          a(0)=0, a(1)=1.
 * https://oeis.org/A270657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(0)=0, a(1)=1. For n > 1: if n % 3 == 0 : a(n) = a(n/3) if n %
 *            3 == 1 : a(n) = a((n+2)/3) - a((n-1)/3) // a(n)=a(k+1)-a(k) if n
 *            % 3 == 2 : a(n) = a((n-2)/3) + a((n+1)/3) // a(n)=a(k)+a(k+1)
 *            This is a direct conversion of the OEIS recurrence and can be
 *            evaluated iteratively in O(n)
 * Range:     i8 n <= 9212, i16 n <= 9212, i32 n <= 9212, i64 n <= 9212,
 *            i128 n <= 9212, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store previously computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* current capacity of cache            */
    static idx_t  last  = 0;      /* highest index computed so far         */

    /* ensure the cache holds indices up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
        /* initialise the base values that may be needed */
        if (cap > 0) cache[0] = (seq_t)0;
        if (cap > 1) cache[1] = (seq_t)1;
    }

    /* compute values up to n if necessary */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)0;
            } else if (i == 1) {
                cache[1] = (seq_t)1;
            } else {
                idx_t r = i % (idx_t)3u;
                if (r == 0u) {
                    cache[i] = cache[i / (idx_t)3u];
                } else if (r == 1u) {
                    idx_t k = (i - (idx_t)1u) / (idx_t)3u;  /* k = (i-1)/3 */
                    cache[i] = cache[k + (idx_t)1u] - cache[k];
                } else { /* r == 2u */
                    idx_t k = (i - (idx_t)2u) / (idx_t)3u;  /* k = (i-2)/3 */
                    cache[i] = cache[k] + cache[k + (idx_t)1u];
                }
            }
        }
        last = n;
    }

    return cache[n];
}
