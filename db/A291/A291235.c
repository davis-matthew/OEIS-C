/*
 * A291235  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - S - S^2 - S^3 -
 *          S^4 - S^5.
 * https://oeis.org/A291235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291235: a(n) = a(n-1) + 6*a(n-2) - 3*a(n-3) - 12*a(n-4) + 5*a(n-5) + 12*a(n-6)
   - 3*a(n-7) - 6*a(n-8) + a(n-9) + a(n-10)  for n >= 11
   Initial values: a(0)=1,a(1)=2,a(2)=5,a(3)=12,a(4)=29,a(5)=69,a(6)=166,a(7)=394,
   a(8)=944,a(9)=2245,a(10)=5365
*/
#include <stdlib.h>

static const seq_t init[11] = {
    1, 2, 5, 12, 29, 69, 166, 394, 944, 2245, 5365
};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cached_upto = 0;     /* number of computed values = cached_upto */
    static size_t cache_cap = 0;        /* allocated size of cache */

    /* Ensure cache size is at least n+1 */
    if (cached_upto <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 11;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */

#if defined(__cplusplus)
        seq_t *new_cache = new seq_t[new_cap];
        /* copy old data */
        for (size_t i = 0; i < cached_upto; ++i) new_cache[i] = cache[i];
        delete [] cache;
        cache = new_cache;
#else
        seq_t *new_cache = realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
#endif
        cache_cap = new_cap;

        /* compute missing values up to n */
        for (size_t i = cached_upto; i <= (size_t)n; ++i) {
            if (i < 11) {
                cache[i] = init[i];
            } else {
                seq_t val = cache[i-1];
                val += cache[i-2] * (seq_t)6;
                val -= cache[i-3] * (seq_t)3;
                val -= cache[i-4] * (seq_t)12;
                val += cache[i-5] * (seq_t)5;
                val += cache[i-6] * (seq_t)12;
                val -= cache[i-7] * (seq_t)3;
                val -= cache[i-8] * (seq_t)6;
                val += cache[i-9];
                val += cache[i-10];
                cache[i] = val;
            }
        }
        cached_upto = n + 1;
    }
    return cache[(size_t)n];
}
