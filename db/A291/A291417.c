/*
 * A291417  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - 4 S + 2 S^2.
 * https://oeis.org/A291417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=4, a(1)=18, a(2)=76, a(3)=322, a(4)=1360
 *            a(n)=4*a(n-1)+2*a(n-2)-4*a(n-3)-2*a(n-4) for n>=5 (OEIS A291417)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* The first five terms are known */
    static const seq_t init[5] = {4, 18, 76, 322, 1360};

    if (n < 5)
        return init[(size_t)n];

    /* initialise cache if needed */
    if (cache == NULL) {
        cache_cap = 5;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
        cache[4] = init[4];
    }

    /* grow cache if we need more space */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp != NULL) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* compute missing terms up to n */
    for (size_t i = 5; i <= (size_t)n; ++i) {
        seq_t val = 4 * cache[i - 1] + 2 * cache[i - 2]
                    - 4 * cache[i - 3] - 2 * cache[i - 4];
        cache[i] = val;
    }

    return cache[(size_t)n];
}
