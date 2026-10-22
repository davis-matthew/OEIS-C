/*
 * A359620  Number of edge cuts in the n-antiprism graph.
 * https://oeis.org/A359620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42*a(n-1) - 617*a(n-2) + 3640*a(n-3) - 7144*a(n-4) +
 *            5888*a(n-5) - 2064*a(n-6) + 256*a(n-7); Initial values: a(0)=1,
 *            a(1)=4, a(2)=62, a(3)=1440, a(4)=30346, a(5)=589556,
 *            a(6)=10858046
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static int initialized = 0;
    static const seq_t init[7] = {(seq_t)1, (seq_t)4, (seq_t)62, (seq_t)1440,
                                  (seq_t)30346, (seq_t)589556, (seq_t)10858046};

    /* Ensure the cache can hold at least n+1 entries, plus the 7 initial terms */
    size_t need_cap = (size_t)n + 1;
    if (need_cap < 7u)
        need_cap = 7u;
    if (cache_cap < need_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = need_cap;
    }

    /* Initialize the first seven terms once */
    if (!initialized) {
        for (size_t j = 0; j < 7; ++j)
            cache[j] = init[j];
        initialized = 1;
    }

    if (n < 7)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t pos = cache[i-1];
        pos *= 42;
        seq_t tmp = cache[i-3];
        tmp *= 3640;
        pos += tmp;
        tmp = cache[i-5];
        tmp *= 5888;
        pos += tmp;
        tmp = cache[i-7];
        tmp *= 256;
        pos += tmp;

        seq_t neg = cache[i-2];
        neg *= 617;
        tmp = cache[i-4];
        tmp *= 7144;
        neg += tmp;
        tmp = cache[i-6];
        tmp *= 2064;
        neg += tmp;

        cache[i] = pos - neg;
    }

    return cache[n];
}
