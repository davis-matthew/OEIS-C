/*
 * A311416  Coordination sequence Gal.4.48.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - 3*a(n-2) + 4*a(n-3) - 5*a(n-4) + 6*a(n-5)
   - 7*a(n-6) + 8*a(n-7) - 8*a(n-8) + 8*a(n-9) - 7*a(n-10) + 6*a(n-11)
   - 5*a(n-12) + 4*a(n-13) - 3*a(n-14) + 2*a(n-15) - a(n-16);
   a(0)=1; a(1)=4; a(2)=8; a(3)=12; a(4)=17; a(5)=21; a(6)=24;
   a(7)=29; a(8)=35; a(9)=40; a(10)=43; a(11)=45; a(12)=49;
   a(13)=54; a(14)=58; a(15)=62; a(16)=67; */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

static const seq_t init_vals[17] = {
    1, 4, 8, 12, 17, 21, 24, 29, 35, 40,
    43, 45, 49, 54, 58, 62, 67
};

seq_t a(idx_t n)
{
    /* Initialise cache if it hasn't been done yet */
    if (cache_len == 0) {
        size_t len = 17;
        cache = (seq_t *)malloc(len * sizeof(seq_t));
        if (!cache) { /* allocation failed – return 0 as a safe fallback */
            return 0;
        }
        for (size_t i = 0; i < len; ++i) {
            cache[i] = init_vals[i];
        }
        cache_len = len;
    }

    /* Extend cache if required */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* realloc failed – keep old cache, return 0 */
            return 0;
        }
        cache = new_cache;
        /* Compute values from old cache_len up to n */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* recurrence valid only for i >= 17 */
            seq_t val = 2 * cache[i - 1]
                  - 3 * cache[i - 2]
                  + 4 * cache[i - 3]
                  - 5 * cache[i - 4]
                  + 6 * cache[i - 5]
                  - 7 * cache[i - 6]
                  + 8 * cache[i - 7]
                  - 8 * cache[i - 8]
                  + 8 * cache[i - 9]
                  - 7 * cache[i -10]
                  + 6 * cache[i -11]
                  - 5 * cache[i -12]
                  + 4 * cache[i -13]
                  - 3 * cache[i -14]
                  + 2 * cache[i -15]
                  - 1 * cache[i -16];
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
