/*
 * A259718  Number of (n+2)X(3+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 or 00000011.
 * https://oeis.org/A259718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) + 6*a(n-4) - 9*a(n-6) - 16*a(n-7) - 12*a(n-8) +
 *            4*a(n-9) + 7*a(n-10) + 10*a(n-11) + 14*a(n-12) - 3*a(n-16)
 *            Initial values (a(1) .. a(16)): 140, 228, 568, 1064, 1244, 2858,
 *            6092, 8638, 15640, 34858, 57256, 94682, 200008, 364268, 598252,
 *            1171728
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 29, u64 n <= 66, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[16] = {
    (seq_t)140, (seq_t)228, (seq_t)568, (seq_t)1064, (seq_t)1244,
    (seq_t)2858, (seq_t)6092, (seq_t)8638, (seq_t)15640, (seq_t)34858,
    (seq_t)57256, (seq_t)94682, (seq_t)200008, (seq_t)364268,
    (seq_t)598252, (seq_t)1171728
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    if (n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failed */
        cache = new_cache;

        /* Compute newly needed values */
        for (idx_t i = cache_cap; i < new_cap; ++i) {
            if (i < 16) {
                cache[i] = init_vals[i];
            } else {
                seq_t res = 0;
                res +=  (seq_t)6 * cache[i - 3];
                res +=  (seq_t)6 * cache[i - 4];
                res -=  (seq_t)9 * cache[i - 6];
                res -= (seq_t)16 * cache[i - 7];
                res -= (seq_t)12 * cache[i - 8];
                res +=  (seq_t)4 * cache[i - 9];
                res +=  (seq_t)7 * cache[i - 10];
                res += (seq_t)10 * cache[i - 11];
                res += (seq_t)14 * cache[i - 12];
                res -=  (seq_t)3 * cache[i - 16];
                cache[i] = res;
            }
        }
        cache_cap = new_cap;
    }

    return cache[n - 1];
}
