/*
 * A253433  Number of (n+1) X (6+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) for n > 3, a(1) = 325, a(2) = 318,
 *            a(3) = 336. (Equivalently, a(n) = 9*2^(n-1) + 300 for n > 1.)
 * Range:     u8 none, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed terms; indices 1..max_computed */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    if (n == 0) return 0;              /* offset is 1; value for 0 unused */
    /* Ensure the cache can hold term n */
    if (n > max_computed) {
        /* Grow cache (at least capacity enough for n+1 entries) */
        size_t cap = max_computed == 0 ? 4 : max_computed * 2;
        while (cap < n + 1) cap <<= 1;

        seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!new_cache) {              /* allocation failure – abort safely */
            return 0;
        }
        cache = new_cache;

        /* Compute missing terms */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)325;
            } else if (i == 2) {
                val = (seq_t)318;
            } else if (i == 3) {
                val = (seq_t)336;
            } else {
                val = 3 * cache[i - 1] - 2 * cache[i - 2];
            }
            cache[i] = val;
        }
        max_computed = n;
    }
    return cache[n];
}
