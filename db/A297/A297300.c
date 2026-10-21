/*
 * A297300  Number of 3 X n 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 2 neighboring 1s.
 * https://oeis.org/A297300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - a(n-4) (n>6) a(1)=1,
 *            a(2)=1, a(3)=3, a(4)=5, a(5)=9, a(6)=17
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 71, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* below offset, value irrelevant */

    /* Small n handled directly */
    if (n < 7) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)1;
            case 3: return (seq_t)3;
            case 4: return (seq_t)5;
            case 5: return (seq_t)9;
            case 6: return (seq_t)17;
        }
    }

    /* Dynamic cache for n >= 7 */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_max = 0;

    if (cache == NULL) {
        /* Allocate and initialise base values */
        cache_cap = n + 1;   /* at least up to n */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely */

        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)9;
        cache[6] = (seq_t)17;
        cache_max = 6;
    } else if (n + 1 > cache_cap) {
        /* Grow cache to accommodate the requested index */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        for (idx_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    if (cache_max < n) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t val = (cache[i - 1] << 1) - cache[i - 2]
                        + (cache[i - 3] << 1) - cache[i - 4];
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
