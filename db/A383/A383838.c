/*
 * A383838  Expansion of 1/((1-x) * (1-4*x) * (1-9*x) * (1-16*x)).
 * https://oeis.org/A383838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*a(n-1) - 273*a(n-2) + 820*a(n-3) - 576*a(n-4), a(0) =
 *            1, a(1) = 30, a(2) = 627, a(3) = 11440
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cap = 0;     /* allocated capacity */
    static idx_t computed = 0;/* number of terms already computed (cache[0..computed-1]) */

    /* Ensure the cache is large enough */
    if (n + (idx_t)1 > cap) {
        idx_t new_cap = cap ? cap << 1 : (idx_t)4;
        if (new_cap < n + (idx_t)1) new_cap = n + (idx_t)1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = computed; k <= n; ++k) {
        if (k == 0)          cache[0] = 1;
        else if (k == 1)      cache[1] = 30;
        else if (k == 2)      cache[2] = 627;
        else if (k == 3)      cache[3] = 11440;
        else
            cache[k] = 30 * cache[k-1] - 273 * cache[k-2]
                       + 820 * cache[k-3] - 576 * cache[k-4];
    }
    computed = n + (idx_t)1;

    return cache[n];
}
