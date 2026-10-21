/*
 * A271176  Expansion of -(4*x^3-7*x^2+4*x-1)/(2*x^4-5*x^3+8*x^2-5*x+1).
 * https://oeis.org/A271176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=5*a(n-1)-8*a(n-2)+5*a(n-3)-2*a(n-4) for n>3,
   a(0)=1, a(1)=1, a(2)=4, a(3)=13 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache for computed terms   */
    static size_t cache_cap = 0;         /* current cache capacity    */
    static seq_t base[4] = {1, 1, 4, 13};/* value for n=0..3           */

    if (n < 4) {                         /* trivial cases             */
        return base[n];
    }

    /* Allocate the cache once and initialise the first four terms */
    if (cache_cap < 4) {
        cache_cap = 4;
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = base[0];
        cache[1] = base[1];
        cache[2] = base[2];
        cache[3] = base[3];
    }

    size_t idx = (size_t)n;

    /* Grow the cache or compute new terms up to n */
    if (cache_cap <= idx) {
        size_t new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = cache_cap; i <= idx; ++i) {
            cache[i] = (seq_t)5 * cache[i-1]
                     - (seq_t)8 * cache[i-2]
                     + (seq_t)5 * cache[i-3]
                     - (seq_t)2 * cache[i-4];
        }
        cache_cap = new_cap;
    }

    return cache[idx];
}
