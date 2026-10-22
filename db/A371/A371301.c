/*
 * A371301  Expansion of (x^2 + x - 1)/((1 - x)*(2*x^2 + 3*x - 1)). Row sums
 *          of A371300.
 * https://oeis.org/A371301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Expansion of (x^2 + x - 1)/((1 - x)*(2*x^2 + 3*x - 1))
 * Sequence satisfies the recurrence:
 *   a(0) = 1, a(1) = 3, a(2) = 10
 *   a(n) = 4*a(n-1) - a(n-2) - 2*a(n-3)   for n ≥ 3
 */
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n) {
    /* static memoisation storage */
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_len = 0;  /* number of terms actually computed */

    /* ensure capacity to store indices 0 … n */
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        grow_seq_array(&cache, &cache_cap, need);
    }

    /* compute missing terms up to n */
    if (cache_len < need) {
        for (size_t i = cache_len; i < need; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)3;
            } else if (i == 2) {
                cache[2] = (seq_t)10;
            } else {
                seq_t t = cache[i - 1];
                t = t * (seq_t)4;
                t = t - cache[i - 2];
                t = t - (cache[i - 3] * (seq_t)2);
                cache[i] = t;
            }
        }
        cache_len = need;
    }

    return cache[(size_t)n];
}
