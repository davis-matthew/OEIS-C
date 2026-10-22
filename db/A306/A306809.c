/*
 * A306809  Binomial transform of the continued fraction expansion of e.
 * https://oeis.org/A306809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            7*a(n-1)-21*a(n-2)+37*a(n-3)-43*a(n-4)+33*a(n-5)-16*a(n-6)+4*a(n-7)
 *            for n>6, with a(0)=2, a(1)=3, a(2)=6, a(3)=12, a(4)=23, a(5)=46,
 *            a(6)=98
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache to guarantee correct value for any ordering of calls */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;      /* allocated size */
    static size_t computed = 0;      /* number of terms stored (<= cache_max) */

    /* Initialise cache and first 7 terms if necessary */
    if (cache == NULL) {
        size_t init = 7;
        if ((idx_t)(n + 1) > init) init = n + 1;
        /* Allocate array; explicit cast needed for C++ compilers */
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failed – unlikely */

        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)23;
        cache[5] = (seq_t)46;
        cache[6] = (seq_t)98;
        cache_max = (idx_t)init;
        computed = 7;
    }

    /* Return immediately if already known */
    if (n < computed) return cache[n];

    /* Grow the cache if needed (doubling strategy) */
    if ((idx_t)(n + 1) > cache_max) {
        idx_t new_max = cache_max;
        while ((idx_t)(n + 1) > new_max) new_max <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_max * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failed – unlikely */
        cache = tmp;
        cache_max = new_max;
    }

    /* Compute missing terms up to n */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        if (i < 7) continue;   /* safety; should not happen */

        seq_t val = (seq_t)7 * cache[i-1]
                    - (seq_t)21 * cache[i-2]
                    + (seq_t)37 * cache[i-3]
                    - (seq_t)43 * cache[i-4]
                    + (seq_t)33 * cache[i-5]
                    - (seq_t)16 * cache[i-6]
                    + (seq_t)4  * cache[i-7];
        cache[i] = val;
    }
    computed = (size_t)n + 1;
    return cache[n];
}
