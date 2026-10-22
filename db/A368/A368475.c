/*
 * A368475  Expansion of o.g.f. (1-x)^5/((1-x)^5 - x^4).
 * https://oeis.org/A368475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 4*a(n-4) + a(n-5) for
 *            n >= 6; a(0)=1, a(1)=a(2)=a(3)=0, a(4)=1, a(5)=5
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 31, u64 n <= 60, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* if n is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* allocate or grow the cache to hold up to n */
    idx_t new_len = n + 1;
    seq_t *tmp;
    if (cache_len == 0) {
        tmp = (seq_t *)malloc(sizeof(seq_t) * new_len);
    } else {
        tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    }
    if (!tmp) {
        /* allocation failed – abort */
        abort();
    }
    cache = tmp;

    /* initialise or compute the new entries */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1 || i == 2 || i == 3) {
            cache[i] = 0;
        } else if (i == 4) {
            cache[4] = 1;
        } else if (i == 5) {
            cache[5] = 5;
        } else {
            cache[i] = 5 * cache[i - 1]
                       - 10 * cache[i - 2]
                       + 10 * cache[i - 3]
                       - 4 * cache[i - 4]
                       + cache[i - 5];
        }
    }

    cache_len = new_len;
    return cache[n];
}
