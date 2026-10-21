/*
 * A291265  a(n) = (1/3)*A291232(n).
 * https://oeis.org/A291265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) - 6*a(n-3) - a(n-4) (n ≥ 5) initial
 *            values: a(0)=2, a(1)=9, a(2)=38, a(3)=153, a(4)=596
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t computed = (idx_t)-1;
    static const seq_t init[5] = {2, 9, 38, 153, 596};

    /* Allocate and seed cache on first call */
    if (cache == NULL) {
        capacity = 5;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init[i];
        computed = 4;   /* last index initialized */
    }

    /* Return cached value if already computed */
    if (n <= computed)
        return cache[(size_t)n];

    /* Ensure the cache is large enough for index n */
    size_t target = (size_t)n;
    if (target + 1 > capacity) {
        size_t newcap = capacity * 2;
        if (newcap < target + 1)
            newcap = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)          /* allocation failed – return last known value */
            return cache[(size_t)computed];
        cache = tmp;
        capacity = newcap;
    }

    /* Compute values sequentially until n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val;
        if (i < 5)
            val = init[(size_t)i];
        else
            val = 6 * cache[(size_t)(i - 1)]
                - 7 * cache[(size_t)(i - 2)]
                - 6 * cache[(size_t)(i - 3)]
                - cache[(size_t)(i - 4)];
        cache[(size_t)i] = val;
    }

    computed = n;
    return cache[(size_t)n];
}
