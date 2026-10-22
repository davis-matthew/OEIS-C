/*
 * A360464  a(n) = a(n-1) + a(n-2) - a(n-3) + gcd(a(n-1), a(n-3)), with a(1) =
 *          a(2) = a(3) = 1.
 * https://oeis.org/A360464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + gcd(a(n-1), a(n-3)), with a(1)
 *            = a(2) = a(3) = 1
 * Range:     u8 n <= 29, u16 n <= 465, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* cache[0] holds a(1), cache[1] holds a(2), ... */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of elements already filled */

    /* Base cases */
    if (n <= 3) return (seq_t)1;

    /* Allocate or enlarge storage if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cache_cap = 3;
    } else if (cache_cap < n) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Fill new terms up to n */
    for (idx_t k = cache_cap; k < n; ++k) {
        cache[k] = cache[k-1] + cache[k-2] - cache[k-3]
                 + gcd(cache[k-1], cache[k-3]);
    }
    cache_cap = n;

    return cache[n-1];
}
