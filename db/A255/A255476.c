/*
 * A255476  A255475(2^n-1).
 * https://oeis.org/A255476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence based on the rational generating function:
   a(0)=1, a(1)=6, a(2)=24, a(3)=94, a(4)=356,
   a(5)=1338, a(6)=5036, a(7)=18942, a(8)=71264, a(9)=268114.
   For n ≥ 10:
   a(n)=3*a(n-1)+6*a(n-2)-9*a(n-3)-9*a(n-4)-7*a(n-5)+2*a(n-6)
        +15*a(n-7)-4*a(n-8)-8*a(n-9)-8*a(n-10); */

#include <stdlib.h>
#include "../utils/seq_realloc.h"   /* provides seq_realloc(seq_t*,size_t,size_t) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* current number of cached terms */

    /* Ensure capacity for indices up to n. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_len) {
        seq_t *tmp = seq_realloc(cache, cache_len, needed);
        if (!tmp) abort();
        cache = tmp;

        /* Initial values for n = 0 … 9 */
        static const seq_t init[10] = {
            1ULL, 6ULL, 24ULL, 94ULL, 356ULL,
            1338ULL, 5036ULL, 18942ULL, 71264ULL, 268114ULL
        };

        for (size_t i = cache_len; i < needed; ++i) {
            if (i < 10) {
                cache[i] = init[i];
            } else {
                /* Compute a(i) using the recurrence.  All
                   intermediate products fit in seq_t.  */
                cache[i] =
                    (3 * cache[i-1]) + (6 * cache[i-2])
                  + (-9 * cache[i-3]) + (-9 * cache[i-4])
                  + (-7 * cache[i-5]) + (2 * cache[i-6])
                  + (15 * cache[i-7]) + (-4 * cache[i-8])
                  + (-8 * cache[i-9]) + (-8 * cache[i-10]);
            }
        }
        cache_len = needed;
    }

    return cache[n];
}
