/*
 * A247487  Expansion of (2 + x + x^2 + x^3 - x^4 - 2*x^5 - 4*x^6 - 8*x^7) /
 *          (1 - x^4 + 16*x^8) in powers of x.
 * https://oeis.org/A247487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 257, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-4) - 16*a(n-8)   (n ≥ 8)
 * Initial values (offset 0):
 *   a(0)=2, a(1)=1, a(2)=1, a(3)=1, a(4)=1,
 *   a(5)=-1, a(6)=-3, a(7)=-7
 */

#include <stdlib.h>   /* malloc, realloc, free, exit */

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap      = 0;   /* allocated size in elements */
    static size_t computed = 0;   /* number of elements already computed */

    /* Compute values up to n if necessary */
    if ((size_t)n >= computed) {
        size_t needed = (size_t)n + 1;

        /* Grow the cache if required */
        if (needed > cap) {
            size_t new_cap = cap ? cap * 2 : 8;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);   /* allocation failure */
            cache = tmp;
            cap = new_cap;
        }

        /* Populate new elements */
        for (size_t i = computed; i < needed; ++i) {
            if (i < 8) {                     /* initial terms */
                switch ((int)i) {
                    case 0: cache[i] =  2; break;
                    case 1: cache[i] =  1; break;
                    case 2: cache[i] =  1; break;
                    case 3: cache[i] =  1; break;
                    case 4: cache[i] =  1; break;
                    case 5: cache[i] = -1; break;
                    case 6: cache[i] = -3; break;
                    case 7: cache[i] = -7; break;
                }
            } else {                         /* recurrence */
                cache[i] = cache[i-4] - (seq_t)16 * cache[i-8];
            }
        }
        computed = needed;
    }

    return cache[(size_t)n];
}
