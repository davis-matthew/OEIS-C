/*
 * A257557  Expansion (x-1)/(x^5+2*x^3+2*x-1).
 * https://oeis.org/A257557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=14;
   for n>=5: a(n)=2*a(n-1)+2*a(n-3)+a(n-5)   (a(k)=0 for k<0) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t max_computed = 0; /* last computed index (inclusive) */

    if (cache == NULL) {
        /* Allocate at least 5 entries for the base cases */
        cache_size = 5;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        static const seq_t base[5] = {1, 1, 2, 6, 14};
        for (size_t i = 0; i < 5; ++i)
            cache[i] = base[i];
        max_computed = 4;
    }

    if ((idx_t)n > max_computed) {
        /* Ensure capacity */
        if ((size_t)(n + 1) > cache_size) {
            size_t new_size = cache_size;
            while ((size_t)(n + 1) > new_size)
                new_size *= 2;
            cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
            cache_size = new_size;
        }

        /* Compute missing terms */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i <= 4) /* base cases already set */ continue;
            seq_t val = cache[i - 1] * 2;
            val += cache[i - 3] * 2;
            val += cache[i - 5];
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
