/*
 * A331101  Denominators of the best approximations for sqrt(2).
 * https://oeis.org/A331101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(1) = 1, a(2) = 2. For n > 2: if n mod 3 == 2 → a(n) = 3 *
 *            a(n-1) - a(n-2), else → a(n) = a(n-1) + a(n-2)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 38, u64 n <= 76, u128 n <= 151,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* size of allocated array (>= index+1) */
    static idx_t last_computed = 0; /* largest n that has been computed */

    /* initialize cache on first call */
    if (cache == NULL) {
        /* allocate space for at least indices 0..max(3,n) */
        cache_size = (idx_t)(n + 1);
        if (cache_size < 3) cache_size = 3;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* allocation failure */

        /* seed initial terms (indices 1 and 2, index 0 unused) */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        last_computed = 2;
    }

    /* enlarge cache if requested n exceeds current capacity */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* realloc failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i % 3 == 2) {
                cache[i] = 3 * cache[i-1] - cache[i-2];
            } else {
                cache[i] = cache[i-1] + cache[i-2];
            }
        }
        last_computed = n;
    }

    return cache[n];
}
