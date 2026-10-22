/*
 * A361672  a(1) = a(2) = 1; for n > 2, a(n) = a(n-2) + a(n-1) + n if a(n-1)
 *          and n are coprime, otherwise a(n) = a(n-1)/gcd(a(n-1), n).
 * https://oeis.org/A361672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = a(2) = 1; For n > 2: if gcd(a(n-1),n) == 1 then a(n) =
 *            a(n-2) + a(n-1) + n else a(n) = a(n-1) / gcd(a(n-1),n)
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;

    /* ensure the cache is initialized */
    if (cache == NULL) {
        cache_cap = 4;          /* initial capacity, enough for indices 0..3 */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out of memory – unlikely */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_max = 2;
    }

    if (n <= cache_max) {
        return cache[n];
    }

    /* grow cache to accommodate index n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* out of memory – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute from cache_max+1 up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t g = gcd(cache[i - 1], (seq_t)i);
        if (g == (seq_t)1) {
            cache[i] = cache[i - 2] + cache[i - 1] + (seq_t)i;
        } else {
            cache[i] = cache[i - 1] / g;
        }
    }

    cache_max = n;
    return cache[n];
}
