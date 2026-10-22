/*
 * A389124  Upper (1/3,1/2)-midsequence of ((2^n)) and ((3^n)); see Comments.
 * https://oeis.org/A389124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 5*a(n-1) - 5*a(n-2) - 5*a(n-3) + 6*a(n-4)
 * initial terms: a(0)=1, a(1)=3, a(2)=6, a(3)=17
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* fixed initial values for indices 0..3 */
    static const seq_t init_vals[4] = {1, 3, 6, 17};

    /* return directly for the first four terms */
    if (n < 4)
        return init_vals[(idx_t)n];

    /* static cache for memoised results */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated capacity (number of terms) */
    static idx_t last = 3;      /* last index that is already computed */

    /* allocate initial cache on first call (at least 4 slots) */
    if (!cache) {
        cap = 4 > (size_t)n + 1 ? 4 : (size_t)n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return init_vals[0]; /* fallback: should not happen */
        cache[0] = init_vals[0];
        cache[1] = init_vals[1];
        cache[2] = init_vals[2];
        cache[3] = init_vals[3];
        last = 3;
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return init_vals[0]; /* fallback */
        cache = new_cache;
        cap = new_cap;
    }

    /* compute terms up to n */
    if (last < (idx_t)n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t t1 = 5 * cache[i - 1];
            seq_t t2 = 5 * cache[i - 2];
            seq_t t3 = 5 * cache[i - 3];
            seq_t t4 = 6 * cache[i - 4];
            cache[i] = t1 - t2 - t3 + t4;
        }
        last = n;
    }

    return cache[n];
}
