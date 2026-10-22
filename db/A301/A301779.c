/*
 * A301779  Number of nX3 0..1 arrays with every element equal to 0, 1, 2 or 3
 *          horizontally or vertically adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 4*a(n-2); a(1)=4, a(2)=32
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) {
        /* OEIS offset is 1; behaviour for n=0 is irrelevant */
        return (seq_t)0;
    }

#ifdef __cplusplus
    /* C++ implementation using std::vector */
    static std::vector<seq_t> cache;      /* cache[0] unused; indices 1..k stored */
    static idx_t computed = 0;            /* highest index computed so far */

    if (n > computed) {
        if (cache.size() < n + 1) {
            cache.resize(n + 1);
        }
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)4;
            } else if (i == 2) {
                cache[i] = (seq_t)32;
            } else {
                cache[i] = (seq_t)7 * cache[i-1] + (seq_t)4 * cache[i-2];
            }
        }
        computed = n;
    }
    return cache[n];
#else
    /* C implementation using a malloc'ed array */
    static seq_t *cache = NULL;   /* cache[0] unused */
    static idx_t computed = 0;
    static idx_t capacity = 0;    /* number of allocated slots */

    if (n > computed) {
        if ((idx_t)(n + 1) > capacity) {
            capacity = n + 1;
            seq_t *new_cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            if (!new_cache) {
                /* Allocation failure – return 0 as a safe fallback */
                return (seq_t)0;
            }
            cache = new_cache;
        }
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)4;
            } else if (i == 2) {
                cache[i] = (seq_t)32;
            } else {
                cache[i] = (seq_t)7 * cache[i-1] + (seq_t)4 * cache[i-2];
            }
        }
        computed = n;
    }
    return cache[n];
#endif
}
