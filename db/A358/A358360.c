/*
 * A358360  The 3-adic valuation of the central Delannoy numbers (sequence
 *          A001850).
 * https://oeis.org/A358360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; For n ≥ 1: If n ≡ 1 (mod 3) a(n) = a(⌊n/9⌋) + 1 If n ≡
 *            0 or 2 (mod 3) a(n) = a(⌊n/3⌋) + (⌊n/3⌋ mod 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* lazy cache of already computed terms */
    static seq_t *cache = NULL;   /* array of seq_t values */
    static idx_t  last   = 0;     /* highest n for which cache[i] is valid   */
    static idx_t  size   = 0;     /* current allocated size (in elements)    */

    /* initialise cache with a(0) */
    if (cache == NULL) {
        size = 1;
        cache = (seq_t *)realloc(NULL, size * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failed */
        cache[0] = (seq_t)0;
        last = 0;
    }

    /* extend the cache if the requested n is beyond the current range */
    if (n > last) {
        /* grow to at least n+1 elements (geometric growth for speed) */
        while (size <= n)
            size = (size == 0) ? 1 : size * 2;
        cache = (seq_t *)realloc(cache, size * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failed */

        /* compute terms from last+1 up to n inclusive */
        for (idx_t i = last + 1; i <= n; ++i) {
            if ((i % 3) == 1) {                 /* n ≡ 1 (mod 3) */
                idx_t base = i / 9;
                cache[i] = cache[base] + (seq_t)1;
            } else {                            /* n ≡ 0 or 2 (mod 3) */
                idx_t f3 = i / 3;
                cache[i] = cache[f3] + (seq_t)(f3 & 1);
            }
        }
        last = n;
    }

    return cache[n];
}
