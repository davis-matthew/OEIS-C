/*
 * A337169  a(n) = (-1)^n + 3 * Sum_{k=0..n-1} a(k) * a(n-k-1).
 * https://oeis.org/A337169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + 3 * Sum_{k=0..n-1} a(k) * a(n-k-1) Recurrence
 *            with initial value a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* Memoized values of a(k) */
    static size_t capacity = 0;   /* Current allocated size */
    static size_t computed = 0;   /* Number of values already computed (cache[0..computed-1]) */

    /* Grow cache if necessary */
    if ((size_t)n >= capacity) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 2 * capacity + 1) new_cap = 2 * capacity + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* Out of memory */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        if (i == 0) {               /* a(0) = 1 */
            cache[0] = 1;
            continue;
        }

        /* Compute S(i-1) = sum_{k=0}^{i-1} a(k)*a(i-1-k) */
        seq_t sum = 0;
        for (size_t k = 0; k < i; ++k) {
            sum += cache[k] * cache[i - 1 - k];
        }

        seq_t term = 3 * sum;       /* 3 * S(i-1) */
        if (i & 1)                   /* i odd: subtract 1 */
            term -= 1;
        else                          /* i even: add 1 */
            term += 1;

        cache[i] = term;
    }

    computed = (size_t)n + 1;       /* All values up to n are now filled */
    return cache[(size_t)n];
}
