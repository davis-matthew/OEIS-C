/*
 * A375163  Expansion of 1/((1 + x)^7 - 2*x^7).
 * https://oeis.org/A375163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 27, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = -7*a(n-1) -21*a(n-2) -35*a(n-3) -35*a(n-4) -
 *              21*a(n-5) -7*a(n-6) + a(n-7);
 * Initial values: a(0)=1, a(1)=-7, a(2)=28, a(3)=-84,
 *                 a(4)=210, a(5)=-462, a(6)=924
 */
#include <stdlib.h>

static const seq_t init[7] = {1, -7, 28, -84, 210, -462, 924};
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    /* Fast path for explicit initial values */
    if (n < 7) return init[(size_t)n];

    /* Ensure cache is initialized and long enough */
    if (cache_len == 0) {
        size_t init_cap = 7;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        cache_len = 7;
    }

    size_t need = (size_t)(n + 1);
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < need) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        seq_t sum = cache[i-7]
                  - (seq_t)7 * cache[i-1]
                  - (seq_t)21 * cache[i-2]
                  - (seq_t)35 * cache[i-3]
                  - (seq_t)35 * cache[i-4]
                  - (seq_t)21 * cache[i-5]
                  - (seq_t)7  * cache[i-6];
        cache[i] = sum;
    }
    cache_len = n + 1;

    return cache[(size_t)n];
}
