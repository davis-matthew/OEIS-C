/*
 * A283903  Relative of Hofstadter Q-sequence.
 * https://oeis.org/A283903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) for n ≥ 19, with initial
 *            values: a(0)=0, a(1)=11, a(2)=20, a(3)=6, a(4)=6, a(5)=6,
 *            a(6)=20, a(7)=11, a(8)=20, a(9)=6, a(10)=6, a(11)=6, a(12)=20,
 *            a(13)=11, a(14)=6, a(15)=12, a(16)=12, a(17)=12, a(18)=20
 * Range:     u8 n <= 101, u16 n <= 2179, u32 n <= 2179, u64 n <= 2179,
 *            u128 n <= 2179, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* a(0)=0 for n≤0 */
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0; /* allocated size */
    static size_t cache_size = 0;     /* last usable index + 1 */

    /* initialise cache on first call */
    if (cache == NULL) {
        size_t init_cap = 19; /* indices 0..18 */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0]  = (seq_t)0;
        cache[1]  = (seq_t)11;
        cache[2]  = (seq_t)20;
        cache[3]  = (seq_t)6;
        cache[4]  = (seq_t)6;
        cache[5]  = (seq_t)6;
        cache[6]  = (seq_t)20;
        cache[7]  = (seq_t)11;
        cache[8]  = (seq_t)20;
        cache[9]  = (seq_t)6;
        cache[10] = (seq_t)6;
        cache[11] = (seq_t)6;
        cache[12] = (seq_t)20;
        cache[13] = (seq_t)11;
        cache[14] = (seq_t)6;
        cache[15] = (seq_t)12;
        cache[16] = (seq_t)12;
        cache[17] = (seq_t)12;
        cache[18] = (seq_t)20;
        cache_capacity = init_cap;
        cache_size = 19; /* next free index is 19 */
    }

    if ((size_t)n < cache_size) {
        return cache[n];
    }

    /* ensure enough space */
    size_t needed = (size_t)n + 1;
    if (needed > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* i ≥ 19 here, because cache_size starts at 19 */
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];

        idx_t idxA = 0, idxB = 0;
        if ((seq_t)i > v1) idxA = (idx_t)((seq_t)i - v1);
        if ((seq_t)i > v2) idxB = (idx_t)((seq_t)i - v2);

        /* idxA and idxB are always ≤ i‑1, so already computed */
        cache[i] = cache[idxA] + cache[idxB];
    }

    cache_size = n + 1;
    return cache[n];
}
