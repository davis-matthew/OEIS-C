/*
 * A268887  Number of 2 X n binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) – 4*a(n-3) – 11*a(n-4) – 6*a(n-5) –
 *            a(n-6) Initial values: a(1)=0, a(2)=5, a(3)=14, a(4)=54,
 *            a(5)=158, a(6)=475
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_max = 0;

    if (n < 1) return 0;

    if (cache == NULL) {
        idx_t cap = (n > 6 ? n : 6);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;
        cache_size = cap;
        cache[0] = 0;
        if (cap > 1) cache[1] = 5;
        if (cap > 2) cache[2] = 14;
        if (cap > 3) cache[3] = 54;
        if (cap > 4) cache[4] = 158;
        if (cap > 5) cache[5] = 475;
    } else if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_size = n;
        /* seed values already present */
    }

    if (computed_max < n) {
        for (idx_t k = computed_max + 1; k <= n; ++k) {
            idx_t idx = k - 1;
            if (k <= 6) {
                /* seed already stored */
                continue;
            }
#ifdef __cplusplus
            seq_t term = 2*cache[idx-1] + 5*cache[idx-2]
                         - 4*cache[idx-3] - 11*cache[idx-4]
                         - 6*cache[idx-5] - cache[idx-6];
#else
            __int128 t = 2*(__int128)cache[idx-1] + 5*(__int128)cache[idx-2]
                         - 4*(__int128)cache[idx-3] - 11*(__int128)cache[idx-4]
                         - 6*(__int128)cache[idx-5] - (__int128)cache[idx-6];
            seq_t term = (seq_t)t;
#endif
            cache[idx] = term;
        }
        computed_max = n;
    }

    return cache[n-1];
}
