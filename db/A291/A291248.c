/*
 * A291248  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - S - S^2 - S^3 -
 *          S^4 + S^5.
 * https://oeis.org/A291248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 6*a(n-2) - 3*a(n-3) - 12*a(n-4) + 3*a(n-5) * +
 *            12*a(n-6) - 3*a(n-7) - 6*a(n-8) + a(n-9) + a(n-10) * for n ≥ 11,
 *            with a(0) = 1, a(1) = 2, a(2) = 5, a(3) = 12, * a(4) = 27, a(5)
 *            = 65, a(6) = 146, a(7) = 346, a(8) = 788, * a(9) = 1845, a(10) =
 *            4239
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[] = {1, 2, 5, 12, 27, 65, 146, 346, 788, 1845, 4239};
static const idx_t init_len = sizeof(init)/sizeof(init[0]);

static seq_t *cache = NULL;
static idx_t cap = 0;
static idx_t computed = 0;

seq_t a(idx_t n) {
    /* Return known initial terms without allocating memory */
    if (n < init_len) {
        return init[n];
    }

    /* Allocate or grow the cache as needed */
    if (!cache) {
        cap = 16;
        if (cap < n + 1) {
            while (cap < n + 1) cap *= 2;
        }
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i < init_len; ++i) cache[i] = init[i];
        computed = init_len - 1;   /* 10 */
    } else if (cap < n + 1) {
        idx_t newcap = cap ? cap * 2 : 16;
        while (newcap < n + 1) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
        /* Newly allocated slots are untouched – no need to init */
    }

    /* Compute missing terms up to n */
    if (n > computed) {
        /* Ensure we have values up to 10 before we start recurrence */
        if (computed < init_len - 1) {
            for (idx_t i = computed + 1; i < init_len; ++i)
                cache[i] = init[i];
            computed = init_len - 1;
        }
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t val = cache[i-1]
                      + 6 * cache[i-2]
                      - 3 * cache[i-3]
                      - 12 * cache[i-4]
                      + 3 * cache[i-5]
                      + 12 * cache[i-6]
                      - 3 * cache[i-7]
                      - 6 * cache[i-8]
                      + cache[i-9]
                      + cache[i-10];
            cache[i] = val;
        }
        computed = n;
    }

    return cache[n];
}
