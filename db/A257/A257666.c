/*
 * A257666  Number of ways of placing n balls into boxes 1,2,... in such a way
 *          that any two adjacent boxes contain at least 4 balls.
 * https://oeis.org/A257666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Recurrence order 9:
 *            a(0)=1,a(1)=1,a(2)=1,a(3)=1,a(4)=7,a(5)=11,a(6)=17,a(7)=25,a(8)=51,a(9)=94
 *            a(n) = a(n-2)+a(n-3)+3*a(n-4)+3*a(n-5)+a(n-6)-a(n-8)-a(n-9) for
 *            n ≥ 10
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 79, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[10] = {
    (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)7,
    (seq_t)11, (seq_t)17, (seq_t)25, (seq_t)51, (seq_t)94
};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    if ((idx_t)(n + 1) > cache_size) {
        const idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 10) {
                cache[i] = init_vals[i];
            } else {
                cache[i] =
                    cache[i - 2] + cache[i - 3] +
                    (seq_t)3 * cache[i - 4] + (seq_t)3 * cache[i - 5] +
                    cache[i - 6] - cache[i - 8] - cache[i - 9];
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
