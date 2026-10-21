/*
 * A280865  Expansion of 1/(1 - Sum_{k>=0} x^((2*k+1)^3)).
 * https://oeis.org/A280865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into odd cubes. a(0) = 1, for
 *            n > 0: a(n) = Σ a(n - m³), where m is odd and m³ ≤ n
 * Range:     u8 n <= 72, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/int_cube_root.h"   /* provides idx_t int_cube_root(idx_t) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[i] = a(i) */
    static idx_t cache_size = 0;     /* current allocated size */

    /* initialise cache */
    if (cache == NULL) {
#if defined(__cplusplus)
        cache = new seq_t[1];
#else
        cache = (seq_t *)malloc(sizeof(seq_t));
#endif
        if (cache == NULL) return 0;      /* allocation failure */
        cache[0] = (seq_t)1;              /* a(0) = 1 */
        cache_size = 1;
    }

    /* grow cache if necessary */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *new_cache;

#if defined(__cplusplus)
        new_cache = new seq_t[new_size];
#else
        new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
#endif
        if (new_cache == NULL) return 0;   /* allocation failure */

        /* copy existing values */
        for (idx_t i = 0; i < old_size; ++i)
            new_cache[i] = cache[i];

#if defined(__cplusplus)
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_size = new_size;

        /* compute newly needed entries */
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            idx_t max_m = int_cube_root(i);        /* floor(cuberoot(i)) */
            for (idx_t m = 1; m <= max_m; m += 2) {   /* odd m */
                idx_t cube = m * m * m;              /* (2k+1)³ = m³ */
                sum += cache[i - cube];
            }
            cache[i] = sum;
        }
    }

    return cache[n];
}
