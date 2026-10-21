/*
 * A264286  Number of (2+1) X (n+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change 0,0 0,1 1,0 or -1,-2.
 * https://oeis.org/A264286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 4*a(n-2) + 10*a(n-3) + 12*a(n-4) + 8*a(n-5)  for n > 7
 * Initial values: a(1)=1, a(2)=10, a(3)=32, a(4)=102, a(5)=360, a(6)=1228, a(7)=4156
 */

#include <stdlib.h>
#include "../utils/mul_small.h"   /* provides mul_small(seq_t, unsigned) */

seq_t a(idx_t n)
{
    /* 1‑based cache of computed terms */
    static seq_t *cache          = NULL;
    static idx_t cache_cap       = 0;   /* allocated size (≥ current index + 1) */
    static idx_t computed_up_to  = 0;   /* largest index for which cache is filled */

    if (n < 1) return (seq_t)0;  /* undefined, but keep safe */

    /* Grow the cache if necessary */
    if ((idx_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 128) new_cap = 128;          /* spare slack */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (cache == NULL) { /* allocation failed – return 0 as safe fallback */
            return (seq_t)0;
        }
        cache_cap = (idx_t)new_cap;
    }

    /* Bootstrap the first seven terms once */
    if (computed_up_to < 7) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)32;
        cache[4] = (seq_t)102;
        cache[5] = (seq_t)360;
        cache[6] = (seq_t)1228;
        cache[7] = (seq_t)4156;
        computed_up_to = 7;
    }

    /* Compute any missing terms using the recurrence */
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            seq_t val = cache[i - 1];
            val += mul_small(cache[i - 2], (unsigned)4);
            val += mul_small(cache[i - 3], (unsigned)10);
            val += mul_small(cache[i - 4], (unsigned)12);
            val += mul_small(cache[i - 5], (unsigned)8);
            cache[i] = val;
        }
        computed_up_to = n;
    }

    return cache[n];
}
