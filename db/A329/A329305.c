/*
 * A329305  Denominators of convergents to A309930, the constant whose
 *          continued fraction representation consists of the cubes, [0; 1, 8,
 *          27, 64, ...].
 * https://oeis.org/A329305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^3*a(n-1) + a(n-2) for n>=2, a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms currently stored */
    static idx_t cache_cap  = 0;   /* allocated capacity */

    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = 1;
        } else {
            seq_t cube = (seq_t)i * (seq_t)i * (seq_t)i;
            cache[i] = cube * cache[i-1] + cache[i-2];
        }
    }
    cache_size = n + 1;

    return cache[n];
}
