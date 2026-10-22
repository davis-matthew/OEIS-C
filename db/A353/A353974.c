/*
 * A353974  a(n) is the n-th partial sum of A056992.
 * https://oeis.org/A353974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) (for n > 9) * initial values:
 *            a(0)=0, a(1)=1, a(2)=5, a(3)=14, a(4)=21, a(5)=28, * a(6)=37,
 *            a(7)=41, a(8)=42, a(9)=51
 * Range:     u8 n <= 45, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial values for indices 0..9 (inclusive) */
    static const seq_t init_vals[10] = {
        0, 1, 5, 14, 21, 28, 37, 41, 42, 51
    };

    /* Static cache: dynamic array holding already computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity (number of elements) */
    static size_t len = 0;   /* number of terms that are already initialized */

    /* For n < 10 the answer is known directly */
    if (n < 10u) {
        return init_vals[(size_t)n];
    }

    /* First use of the cache: allocate, copy the first 10 terms, set len */
    if (cache == NULL) {
        /* Ensure a reasonable starting capacity */
        cap = (size_t)n + 1;
        if (cap < 16u) cap = 16u;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();

        for (size_t i = 0; i < 10u; ++i) {
            cache[i] = init_vals[i];
        }
        len = 10u;
    }

    size_t target = (size_t)n;

    /* Grow the cached array if necessary */
    if (target + 1 > cap) {
        size_t new_cap = cap ? cap * 2u : 16u;
        if (new_cap < target + 1) new_cap = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            free(cache);
            abort();
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = len; i <= target; ++i) {
        /* recurrence: a(i) = a(i-1) + a(i-9) - a(i-10) */
        cache[i] = cache[i - 1] + cache[i - 9] - cache[i - 10];
    }
    len = target + 1u;

    return cache[target];
}
