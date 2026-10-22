/*
 * A302636  Number of 3Xn 0..1 arrays with every element equal to 0, 1, 3 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+2*a(n-4)-a(n-5) for n>7,
 *            a(1)=4,a(2)=6,a(3)=8,a(4)=14,a(5)=25,a(6)=47,a(7)=83
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 37, u64 n <= 74, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* maximum index computed (>=0) */
    static size_t cache_cap = 0; /* allocated capacity */

    if (n == 0) return 0; /* offset starts at 1; undefined for 0 */

    size_t idx = (size_t)n;

    /* ensure capacity */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    } else if (idx >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= idx) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute values up to idx */
    for (size_t i = cache_len + 1; i <= idx; ++i) {
        seq_t val;
        if (i <= 7) {
            /* initial values: index 1..7 */
            static const seq_t init[8] = {0, 4, 6, 8, 14, 25, 47, 83};
            val = init[i];
        } else {
            val = cache[i - 1] + cache[i - 2] + cache[i - 4] * 2 - cache[i - 5];
        }
        cache[i] = val;
    }
    cache_len = idx;

    return cache[idx];
}
