/*
 * A250907  Number of (n+1) X (2+1) 0..2 arrays with no 2 X 2 subblock having
 *          its maximum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A250907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) - 41*a(n-2) + 36*a(n-3) Initial values:
 *            a(1)=603, a(2)=13401, a(3)=297873
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t cached_max = 0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n == 0) return 0;   /* out of offset; not used */

    idx_t idx = n;

    /* initialise cache with the first three terms */
    if (cache == NULL) {
        cache_cap = 8;                      /* arbitrary start size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure */
        cache[0] = 0;                       /* unused index 0 */
        cache[1] = (seq_t)603;
        cache[2] = (seq_t)13401;
        cache[3] = (seq_t)297873;
        cached_max = 3;
    }

    /* grow cache if necessary */
    if ((size_t)idx >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)idx) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to idx */
    if (idx > cached_max) {
        for (idx_t i = cached_max + 1; i <= idx; ++i) {
            seq_t val = 24 * cache[i-1] - 41 * cache[i-2] + 36 * cache[i-3];
            cache[i] = val;
        }
        cached_max = idx;
    }

    return cache[idx];
}
