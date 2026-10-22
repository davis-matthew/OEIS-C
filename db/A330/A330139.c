/*
 * A330139  a(1)=1 and a(2)=1; if a(n-1) + a(n-2) == 0 (mod n) then a(n) =
 *          (a(n-1) + a(n-2))/n else a(n) = a(n-1) + a(n-2).
 * https://oeis.org/A330139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 1; For n > 2: let s = a(n‑1) + a(n‑2); if (s %
 *            n == 0) a(n) = s / n; otherwise a(n) = s
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 54, u64 n <= 100,
 *            u128 n <= 195, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <cstring>   /* for memcpy */
#endif

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;   /* allocated size (index 0 unused) */
    static idx_t  max_calculated = 0;

    /* Ensure n is at least 1 */
    if (n < 1) return 0;

    /* Initial allocation with space for at least indices 1 and 2 */
    if (!cache) {
        size_t init = 4;   /* 0..3, we use 1 and 2 */
        #ifdef __cplusplus
            cache = new seq_t[init];
        #else
            cache = malloc(init * sizeof(seq_t));
        #endif
        if (!cache) return 0;          /* allocation failed */
        cache_size = init;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        max_calculated = 2;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        #ifdef __cplusplus
            seq_t *new_cache = new seq_t[new_size];
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            delete [] cache;
            cache = new_cache;
        #else
            seq_t *new_cache = realloc(cache, new_size * sizeof(seq_t));
            if (!new_cache) return 0;  /* allocation failed */
            cache = new_cache;
        #endif
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_calculated + 1; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        if (sum % (seq_t)i == 0)
            cache[i] = sum / (seq_t)i;
        else
            cache[i] = sum;
    }
    max_calculated = n;

    return cache[n];
}
