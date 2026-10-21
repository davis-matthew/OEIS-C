/*
 * A281710  Number of n X 3 0..1 arrays with no element unequal to a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors
 *          and with new values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence: a(n) = 2*a(n-1) - a(n-3) for n > 6, with
 *            initial values a(1)=1, a(2)=7, a(3)=8, a(4)=14, a(5)=17,
 *            a(6)=22. The function returns a(n) for any n ≥ 1
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base values for 1..6 */
    static const seq_t init[6] = {
        (seq_t)1, (seq_t)7, (seq_t)8,
        (seq_t)14, (seq_t)17, (seq_t)22
    };

    /* Fast return for the first six terms. */
    if (n <= 6u)
        return init[n - 1u];

    /* Static cache to store computed values. */
    static seq_t *cache = NULL;
    static size_t cap = 0;           /* allocated capacity */
    static idx_t last = 0;           /* last index computed (1‑based) */

    /* Allocate or enlarge the cache as needed. */
    if (cache == NULL) {
        cap = (size_t)n;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */
        last = 0;
    } else if (cap < (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cap = (size_t)n;
    }

    /* Ensure the first six terms are stored. */
    if (last < 6u) {
        for (idx_t i = last; i < 6u; ++i)
            cache[i] = init[i];
        last = 6u;
    }

    /* Compute any new terms up to n. */
    if (last < n) {
        for (idx_t i = last + 1u; i <= n; ++i) {
            /* a(i) = 2*a(i-1) - a(i-3)  (indices are 1‑based) */
            cache[i - 1u] = cache[i - 2u] * (seq_t)2 - cache[i - 4u];
        }
        last = n;
    }

    return cache[n - 1u];
}
