/*
 * A303963  Number of nX2 0..1 arrays with every element unequal to 0, 1, 3 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 8*a(n-3) - 4*a(n-4); Initial values: a(1)=2,
 *            a(2)=5, a(3)=9, a(4)=21
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t max_computed = 0;

    /* initialise base values once */
    if (max_computed == 0) {
        size_t need = 5;   /* indices 0..4 */
        if (cache_size < need) {
#ifdef __cplusplus
            seq_t *tmp = new seq_t[need];
#else
            seq_t *tmp = (seq_t *)malloc(need * sizeof(seq_t));
#endif
            if (!tmp) return 0;   /* allocation failure */
            if (cache) {
#ifdef __cplusplus
                delete[] cache;
#else
                free(cache);
#endif
            }
            cache    = tmp;
            cache_size = need;
        }
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)21;
        max_computed = 4;
    }

    if ((idx_t)n <= (idx_t)max_computed) {
        return cache[n];
    }

    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_size];
#else
        seq_t *tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
#endif
        if (!tmp) return 0;
        for (size_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        if (cache) {
#ifdef __cplusplus
            delete[] cache;
#else
            free(cache);
#endif
        }
        cache = tmp;
        cache_size = new_size;
    }

    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] + ((seq_t)8 * cache[i-3]) - ((seq_t)4 * cache[i-4]);
    }
    max_computed = (size_t)n;
    return cache[n];
}
