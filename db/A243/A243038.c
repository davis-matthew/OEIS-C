/*
 * A243038  Number of length n+3 0..2 arrays with no four elements in a row
 *          with pattern abab (with a!=b) and new values 0..2 introduced in
 *          0..2 order.
 * https://oeis.org/A243038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-3) - a(n-4) for n ≥ 5 with a(1)=13,
 *            a(2)=36, a(3)=101, a(4)=285 (Offset for OEIS: a(1) is the first
 *            listed term)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n = 0 */
    if (n == 0) return (seq_t)0;

    /* 1‑based cache: cache[i] stores a(i) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements allocated */
    static idx_t computed = 0;     /* largest index already computed */

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute all missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)13;
        else if (i == 2)
            cache[i] = (seq_t)36;
        else if (i == 3)
            cache[i] = (seq_t)101;
        else if (i == 4)
            cache[i] = (seq_t)285;
        else
            cache[i] = 3 * cache[i - 1] - cache[i - 3] - cache[i - 4];
    }
    computed = n;
    return cache[n];
}
