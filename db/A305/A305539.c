/*
 * A305539  a(n) is a generalized pentagonal number such that 2*a(n) is also a
 *          generalized pentagonal number.
 * https://oeis.org/A305539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3), n > 3 initial values:
 *            a(1)=0, a(2)=1, a(3)=35
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t computed = 0;         /* largest index already computed */
    static seq_t *cache    = NULL;      /* array [0 .. computed-1] of terms */
    static size_t cache_cap = 0;       /* allocated size of cache */

    if (n == 0) return 0;              /* defensive */

    if (n <= computed)                 /* term already cached */
        return cache[n-1];

    /* grow the cache if needed */
    size_t new_cap = (size_t)n;
    if (!cache || new_cap > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)0;
        } else if (i == 2) {
            cache[1] = (seq_t)1;
        } else if (i == 3) {
            cache[2] = (seq_t)35;
        } else {
            cache[i-1] = (seq_t)35 * cache[i-2]   /* 35*a(i-1) */
                       - (seq_t)35 * cache[i-3]   /* 35*a(i-2) */
                       + cache[i-4];             /* +a(i-3)      */
        }
    }

    computed = n;
    return cache[n-1];
}
