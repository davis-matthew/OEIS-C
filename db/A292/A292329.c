/*
 * A292329  p-INVERT of the Fibonacci sequence (A000045), where p(S) = 1 -
 *          S^3.
 * https://oeis.org/A292329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=1, a(3)=3, a(4)=9, a(5)=23, a(6)=57, a(n) =
 *            3*a(n-1) - 4*a(n-3) + 3*a(n-5) + a(n-6) for n>=7
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated slots */
    static idx_t cache_size = 0;  /* last computed index + 1 */

    /* Ensure cache large enough for all indices up to n */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap <= n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap == 0) new_cap = 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise initial terms lazily */
    const seq_t init[7] = { (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)3,
                            (seq_t)9, (seq_t)23, (seq_t)57 };
    if (cache_size < 7) {
        for (idx_t i = cache_size; i < 7 && i <= n; ++i)
            cache[i] = init[i];
        cache_size = (cache_size < 7 ? 7 : cache_size);
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Recurrence: a(n)=3*a(n-1)-4*a(n-3)+3*a(n-5)+a(n-6) */
        seq_t term1 = (seq_t)3 * cache[i-1];
        seq_t term2 = (seq_t)4 * cache[i-3];
        seq_t term3 = (seq_t)3 * cache[i-5];
        seq_t term4 = cache[i-6];
        seq_t sum   = term1 + term3 + term4;   /* 3*a(n-1)+3*a(n-5)+a(n-6)  */
        cache[i] = sum - term2;                /* subtract 4*a(n-3) */
    }
    cache_size = n + 1;

    return cache[n];
}
