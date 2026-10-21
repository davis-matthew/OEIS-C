/*
 * A289150  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 597", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A289150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - a(n-2) - 99*a(n-3) + 100*a(n-4) - 110*a(n-5)
 *            + 100*a(n-6), a(0)=1, a(1)=10, a(2)=110, a(3)=1101, a(4)=11101,
 *            a(5)=111010
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;

    /* enlarge cache if needed                                 */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;

#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
#else
        seq_t *tmp = (seq_t*)malloc(sizeof(seq_t) * new_cap);
#endif
        if (cache) {
            for (idx_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
#ifdef __cplusplus
            delete[] cache;
#else
            free(cache);
#endif
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill missing terms                                       */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)            cache[i] = (seq_t)1;
        else if (i == 1)       cache[i] = (seq_t)10;
        else if (i == 2)       cache[i] = (seq_t)110;
        else if (i == 3)       cache[i] = (seq_t)1101;
        else if (i == 4)       cache[i] = (seq_t)11101;
        else if (i == 5)       cache[i] = (seq_t)111010;
        else {
            seq_t t = cache[i-1] * (seq_t)11
                    + cache[i-4] * (seq_t)100
                    + cache[i-6] * (seq_t)100;

            t = t - cache[i-2];
            t = t - cache[i-3] * (seq_t)99;
            t = t - cache[i-5] * (seq_t)110;

            cache[i] = t;
        }
    }

    cache_len = n + 1;
    return cache[n];
}
