/*
 * A276508  a(n) = (2*5^n + 3*(-1)^(floor((n-1)/3)) + (-1)^n)/6.
 * https://oeis.org/A276508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*5^n + 3*(-1)^(floor((n-1)/3)) + (-1)^n)/6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=2, a(2)=9, a(n)=5*a(n-1)+δ(n) for n≥3 */
/* where δ(n) cycles as 3,2,-1,-3,-2,1 according to n mod 6. */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialize cache with first three terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 2;
        cache[2] = 9;
    }

    /* extend cache if needed */
    size_t idx = (size_t)n;
    if (idx >= cache_len) {
        size_t new_len = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;

        /* delta cycle according to n mod 6 */
        const int delta_arr[6] = {3, 2, -1, -3, -2, 1};

        for (size_t i = cache_len; i < new_len; ++i) {
            int delta = delta_arr[i % 6];
            seq_t temp = 5 * cache[i - 1];
            if (delta >= 0) {
                cache[i] = temp + (seq_t)delta;
            } else {
                cache[i] = temp - (seq_t)(-delta);
            }
        }
        cache_len = new_len;
    }

    return cache[idx];
}
