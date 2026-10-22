/*
 * A316803  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4, 5 or 6 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 3*a(n-2) + 7*a(n-3) - 3*a(n-4) a(1)=4,
 *            a(2)=32, a(3)=247, a(4)=1905
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Guard against undefined index */
    if (n == 0) return (seq_t)0;

    /* Convert index to size_t for array access */
    size_t idx = (size_t)n;

    /* Static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated array size */
    static size_t computed = 0; /* highest index already computed */

    /* Expand the cache array until it can hold idx */
    while (cap <= idx) {
        size_t newcap = (cap == 0) ? 5 : cap * 2;
        if (newcap <= idx) newcap = idx + 1;

        seq_t *tmp;
#ifdef __cplusplus
        tmp = new seq_t[newcap];
        if (cache) {
            for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
            delete[] cache;
        }
#else
        tmp = realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
#endif
        cache = tmp;
        cap   = newcap;
    }

    /* Fast path for the first four terms */
    if (idx <= 4) {
        if (cache[idx] == 0) {
            switch (idx) {
                case 1: cache[1] = (seq_t)4;     break;
                case 2: cache[2] = (seq_t)32;    break;
                case 3: cache[3] = (seq_t)247;   break;
                case 4: cache[4] = (seq_t)1905;  break;
            }
        }
        computed = idx;
        return cache[idx];
    }

    /* Ensure the initial values exist in the array */
    if (cache[1] == 0) cache[1] = (seq_t)4;
    if (cache[2] == 0) cache[2] = (seq_t)32;
    if (cache[3] == 0) cache[3] = (seq_t)247;
    if (cache[4] == 0) cache[4] = (seq_t)1905;

    /* Compute terms that haven't been computed yet */
    if (computed < idx) {
        size_t start = (computed < 5) ? 5 : (computed + 1);
        for (size_t k = start; k <= idx; ++k) {
            seq_t t = cache[k-1] * (seq_t)8;
            t -= cache[k-2] * (seq_t)3;
            t += cache[k-3] * (seq_t)7;
            t -= cache[k-4] * (seq_t)3;
            cache[k] = t;
        }
        computed = idx;
    }

    return cache[idx];
}
