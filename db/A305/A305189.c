/*
 * A305189  a(n) = 1*2 + 3 + 4*5 + 6 + 7*8 + 9 + 10*11 + 12 + ... + (up to n).
 * https://oeis.org/A305189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + ((n%3==2)?(n-1)^2:n), a(1)=1
 * Range:     u8 n <= 13, u16 n <= 82, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms, 1‑based indexing */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size */
    static idx_t last_computed = 0;/* largest n that has been computed */

    if (n == 0) return 0; /* n is always >=1, but guard anyway */

    /* grow cache if necessary */
    if (cache_size < n + 1) {
        idx_t new_size = (n + 1 > cache_size * 2) ? (n + 1) : cache_size * 2;
        if (new_size < n + 1) new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise first term */
    if (last_computed == 0) {
        cache[1] = (seq_t)1;
        last_computed = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t delta;
        if (i % 3 == 2) {
            seq_t x = (seq_t)(i - 1);
            delta = x * x;
        } else {
            delta = (seq_t)i;
        }
        cache[i] = cache[i - 1] + delta;
    }
    last_computed = n;

    return cache[n];
}
